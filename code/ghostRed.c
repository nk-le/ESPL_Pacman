/*
 * ghostRed.c
 *
 *  Created on: Jun 28, 2019
 *      Author: lab_espl_stud11
 */

#include "ghost.h"
#include "Button.h"
#include "Demo.h"
#include "pac_man.h"


//Initialize ghost struct for red ghost
struct ghost ghostRed;

//Local pac-man struct to read pac_man's status
struct pacman pacmanStats_red;

//Initialize ghost red_directions & state
int red_direction=GO_LEFT,red_op_direction=GO_RIGHT,red_opflee_direction=GO_NOWHERE;
int RED_GHOST_STATE = CHASE;
int redFrightTime = 0;

void ghostRedMovementCalculation(){
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	const TickType_t Tick = 20;

	unsigned char collisionStatus = 0;

	// GHOST RED: ****************************************************************************************************************
	ghostRed.x = 112;//starting position for ghost red
	ghostRed.y = 84;
	ghostRed.speed = 1;//default speed of the ghost

	while(1){
		//Halt the action of ghost if it doesn't receive any signal from pacman
		if(xQueueReceive(pacmanStatusToRedQueue, &pacmanStats_red, portMAX_DELAY)==pdTRUE){

			//set ghost's status based on signals from pac_man
			//if(pacmanStats_red.y==RELEASE_RED) RED_GHOST_STATE = GET_OUT;
			if(pacmanStats_red.x==RESET){
				//reset signal received. ghost goes back to original position and reset status
				red_direction = GO_LEFT;
				red_op_direction = GO_RIGHT;
				RED_GHOST_STATE = CHASE;
				redFrightTime = 0;
				ghostRed.x=112;
				ghostRed.y=84;
			}else if(RED_GHOST_STATE==FRIGHT && abs(ghostRed.x-pacmanStats_red.x)<=4 && abs(ghostRed.y-pacmanStats_red.y)<=4){
				//Ghost is eaten
				RED_GHOST_STATE = EATEN;
				collisionStatus = GHOST_EATEN;
				redFrightTime = 0;
				xQueueSend(collisionQueue, &collisionStatus, 0);
				xQueueSend(uartCollisionQueue, &collisionStatus, 0);
			}else if(RED_GHOST_STATE==CHASE && abs(ghostRed.x-pacmanStats_red.x)<=2 && abs(ghostRed.y-pacmanStats_red.y)<=2){
				//Send death signal to pac_man
				collisionStatus = PAC_MAN_DIED;
				xQueueSend(collisionQueue, &collisionStatus, 0);
				xQueueSend(uartCollisionQueue, &collisionStatus, 0);
			}

			if(pacmanStats_red.status==POWERED && (RED_GHOST_STATE==CHASE || RED_GHOST_STATE==FRIGHT)){
				redFrightTime = 1;
				RED_GHOST_STATE = FRIGHT;
				red_op_direction = GO_NOWHERE;
			}else if(redFrightTime == END && RED_GHOST_STATE==FRIGHT){
				redFrightTime = 0;
				RED_GHOST_STATE = CHASE;
				red_opflee_direction = GO_NOWHERE;
			}

			//set ghost movement and red_direction based on ghost state
			switch(RED_GHOST_STATE){
				case LOCKED:
					Red_In_Box();
					break;
				case GET_OUT:
					Get_Out_Red();
					if(ghostRed.x == 112 && ghostRed.y == 84) RED_GHOST_STATE = CHASE;
					break;
				case CHASE:
					Red_Chase();
					break;
				case FRIGHT:
					redFrightTime += 1;
					//execute ghost only half of the cycle, thus slowing down the speed
					if (redFrightTime%2==0)Red_Frightened();
					break;
				case EATEN:
					Red_Eaten();
					if(ghostRed.x == 112 && ghostRed.y == 108) RED_GHOST_STATE = GET_OUT;
					break;
			}

			Draw_Red();
		}

		vTaskDelayUntil(&xLastWakeTime, Tick);
	}
}

//function to calculate ghost's movements inside center box
void Red_In_Box(){
	if(ghostRed.y!=100 && red_direction!=GO_DOWN)ghostRed.y-=ghostRed.speed;
	else if(ghostRed.y!=116 && red_direction!=GO_UP)ghostRed.y+=ghostRed.speed;
	if(ghostRed.y==100) red_direction = GO_DOWN;
	else if(ghostRed.y==116) red_direction = GO_UP;
}

//function to calculate ghost's movements getting out of the box
void Get_Out_Red(){
	//center the ghost
	if(ghostRed.x==112)ghostRed.y-=1;
	else if(ghostRed.y==108){
		if(ghostRed.x!=112)ghostRed.x+=1;
	}
	else if(ghostRed.y<108) ghostRed.y+=1;
	else if(ghostRed.y>108) ghostRed.y-=1;
}

//function to calculate movements to chase pac-man
void Red_Chase(){
	int minIndex=0;
	// CALCULATION OF DISTANCES
	double up = pow((double) (ghostRed.y - 4) - pacmanStats_red.y, 2) + pow((double) ghostRed.x - pacmanStats_red.x, 2);
	double down = pow((double) (ghostRed.y + 4) - pacmanStats_red.y, 2) + pow((double) ghostRed.x - pacmanStats_red.x, 2);
	double right = pow((double) (ghostRed.x + 4) - pacmanStats_red.x, 2) + pow((double) ghostRed.y - pacmanStats_red.y, 2);
	double left = pow((double) (ghostRed.x - 4) - pacmanStats_red.x, 2) + pow((double) ghostRed.y - pacmanStats_red.y, 2);
	double distanceArray[4] = {left,right,up,down};

	//Compare distance of different red_direction and find index of shortest path
	for(int i=0;i<4;i++){
		//if the red_direction is impossible, increase the distance so the index will not be chosen
		if(!checkMovementGhost(ghostRed.x,ghostRed.y,i,0) || i==red_op_direction){
			distanceArray[i]+=3000;
		}else if(distanceArray[i] < distanceArray[minIndex]){
			//if current path is the shortest and also possible
			//mark the index tCHASEo i
			minIndex = i;
		}
	}

	//set the next coordinate to go
	switch(minIndex){
		case GO_RIGHT:
				red_op_direction = GO_LEFT;
				ghostRed.x += ghostRed.speed;
				if(ghostRed.x>=224) ghostRed.x=0;
			break;
		case GO_LEFT:
				red_op_direction = GO_RIGHT;
				ghostRed.x -= ghostRed.speed;
				if(ghostRed.x<=0) ghostRed.x=224;
			break;
		case GO_UP:
				red_op_direction = GO_DOWN;
				ghostRed.y -= ghostRed.speed;
			break;
		case GO_DOWN:
				red_op_direction = GO_UP;
				ghostRed.y += ghostRed.speed;
			break;
		default:
			break;
	}
	red_direction = minIndex;
}

//movement function when ghost is frightened, move away from pacman
void Red_Frightened(){
	// CALCULATION OF DISTANCES
	double up = pow((double) (ghostRed.y - 4) - pacmanStats_red.y, 2) + pow((double) ghostRed.x - pacmanStats_red.x, 2);
	double down = pow((double) (ghostRed.y + 4) - pacmanStats_red.y, 2) + pow((double) ghostRed.x - pacmanStats_red.x, 2);
	double right = pow((double) (ghostRed.x + 4) - pacmanStats_red.x, 2) + pow((double) ghostRed.y - pacmanStats_red.y, 2);
	double left = pow((double) (ghostRed.x - 4) - pacmanStats_red.x, 2) + pow((double) ghostRed.y - pacmanStats_red.y, 2);
	double distanceArray[4] = {left,right,up,down};

	int maxIndex=0;
	//Compare distance of different red_direction and find index of longest path
	for(int i=0;i<4;i++){
		//if the red_direction is impossible, decrease the distance so the index will not be chosen
		if(!checkMovementGhost(ghostRed.x,ghostRed.y,i,0) || i==red_opflee_direction){
			distanceArray[i]=0;
		}else if(distanceArray[i] > distanceArray[maxIndex]){
			//if current path is the longest and also possible
			//mark the index to i
			maxIndex = i;
		}

	}

	//make movement decision based on index of shortest path
	switch(maxIndex){
		case GO_RIGHT:
				red_opflee_direction = GO_LEFT;
				ghostRed.x += ghostRed.speed;
				if(ghostRed.y == 108 && ghostRed.x>=224) ghostRed.x=0;
			break;
		case GO_LEFT:
				red_opflee_direction = GO_RIGHT;
				ghostRed.x -= ghostRed.speed;
				if(ghostRed.y == 108 && ghostRed.x<=0) ghostRed.x=224;
			break;
		case GO_UP:
				red_opflee_direction = GO_DOWN;
				ghostRed.y -= ghostRed.speed;
			break;
		case GO_DOWN:
				red_opflee_direction = GO_UP;
				ghostRed.y += ghostRed.speed;
			break;
		default:
			break;
	}
	red_direction = maxIndex;
}

//movement function when ghost is eaten, flee back to center box
void Red_Eaten(){
	int return_x=112,return_y=108;
	int minIndex=0;
	// CALCULATION OF DISTANCES
	double up = pow((double) (ghostRed.y - 4) - return_y, 2) + pow((double) ghostRed.x - return_x, 2);
	double down = pow((double) (ghostRed.y + 4) - return_y, 2) + pow((double) ghostRed.x - return_x, 2);
	double right = pow((double) (ghostRed.x + 4) - return_x, 2) + pow((double) ghostRed.y - return_y, 2);
	double left = pow((double) (ghostRed.x - 4) - return_x, 2) + pow((double) ghostRed.y - return_y, 2);
	double distanceArray[4] = {left,right,up,down};

	//Compare distance of different red_direction and find index of shortest path
	for(int i=0;i<4;i++){
		//if the red_direction is impossible, increase the distance so the index will not be chosen
		if(!checkMovementGhost(ghostRed.x,ghostRed.y,i,4) || i==red_op_direction){
			distanceArray[i]+=3000;
		}else if(distanceArray[i] < distanceArray[minIndex]){
			//if current path is the shortest and also possible
			//mark the index to i
			minIndex = i;
		}
	}

	//set the next coordinate to go
	switch(minIndex){
		case GO_RIGHT:
				red_op_direction = GO_LEFT;
				ghostRed.x += ghostRed.speed;
				if(ghostRed.y == 108 && ghostRed.x>=224) ghostRed.x=0;
			break;
		case GO_LEFT:
				red_op_direction = GO_RIGHT;
				ghostRed.x -= ghostRed.speed;
				if(ghostRed.y == 108 && ghostRed.x<=0) ghostRed.x=224;
			break;
		case GO_UP:
				red_op_direction = GO_DOWN;
				ghostRed.y -= ghostRed.speed;
			break;
		case GO_DOWN:
				red_op_direction = GO_UP;
				ghostRed.y += ghostRed.speed;
			break;
		default:
			break;
	}
	red_direction = minIndex;
}

void Draw_Red(){
	//draw the ghost based on red_direction and state of the ghost
	if(RED_GHOST_STATE == FRIGHT){
		if(redFrightTime > 475) gdispImageDraw(&WHITE,ghostRed.x-7,ghostRed.y-7,14,14,0,0);
		else if(redFrightTime > 450) gdispImageDraw(&FRIGHTENED,ghostRed.x-7,ghostRed.y-7,14,14,0,0);
		else if(redFrightTime > 425) gdispImageDraw(&WHITE,ghostRed.x-7,ghostRed.y-7,14,14,0,0);
		else if(redFrightTime > 400) gdispImageDraw(&FRIGHTENED,ghostRed.x-7,ghostRed.y-7,14,14,0,0);
		else if(redFrightTime > 375) gdispImageDraw(&WHITE,ghostRed.x-7,ghostRed.y-7,14,14,0,0);
		else if(redFrightTime > 400) gdispImageDraw(&FRIGHTENED,ghostRed.x-7,ghostRed.y-7,14,14,0,0);
		else if(redFrightTime > 375) gdispImageDraw(&WHITE,ghostRed.x-7,ghostRed.y-7,14,14,0,0);
		else if(redFrightTime > 350) gdispImageDraw(&FRIGHTENED,ghostRed.x-7,ghostRed.y-7,14,14,0,0);
		else if(redFrightTime > 325) gdispImageDraw(&WHITE,ghostRed.x-7,ghostRed.y-7,14,14,0,0);
		else if(redFrightTime > 300) gdispImageDraw(&FRIGHTENED,ghostRed.x-7,ghostRed.y-7,14,14,0,0);
		else if(redFrightTime > 275) gdispImageDraw(&WHITE,ghostRed.x-7,ghostRed.y-7,14,14,0,0);
		else gdispImageDraw(&FRIGHTENED,ghostRed.x-7,ghostRed.y-7,14,14,0,0);
	}else{
		switch(red_direction){
			case GO_RIGHT:
					if(RED_GHOST_STATE == EATEN)gdispImageDraw(&EYE_RIGHT,ghostRed.x-7,ghostRed.y-7,14,14,0,0);
					else gdispImageDraw(&RED_RIGHT,ghostRed.x-7,ghostRed.y-7,14,14,0,0);
				break;
			case GO_LEFT:
					if(RED_GHOST_STATE == EATEN)gdispImageDraw(&EYE_LEFT,ghostRed.x-7,ghostRed.y-7,14,14,0,0);
					else gdispImageDraw(&RED_LEFT,ghostRed.x-7,ghostRed.y-7,14,14,0,0);
				break;
			case GO_UP:
					if(RED_GHOST_STATE == EATEN)gdispImageDraw(&EYE_UP,ghostRed.x-7,ghostRed.y-7,14,14,0,0);
					else gdispImageDraw(&RED_UP,ghostRed.x-7,ghostRed.y-7,14,14,0,0);
				break;
			case GO_DOWN:
					if(RED_GHOST_STATE == EATEN)gdispImageDraw(&EYE_DOWN,ghostRed.x-7,ghostRed.y-7,14,14,0,0);
					else gdispImageDraw(&RED_DOWN,ghostRed.x-7,ghostRed.y-7,14,14,0,0);
				break;
			default:
				break;
		}
	}
}
