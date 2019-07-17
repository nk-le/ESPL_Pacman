/*
 * ghostOrange.c
 *
 *  Created on: Jun 28, 2019
 *      Author: lab_espl_stud11
 */
#include "ghost.h"
//#include "gfx.h"
#include "Button.h"
#include "Demo.h"
#include "pac_man.h"
//#include "romfs_files.h"

//gdispImage ORANGE_UP,ORANGE_DOWN,ORANGE_LEFT,ORANGE_RIGHT,FRIGHTENED,
//				EYE_LEFT,EYE_RIGHT,EYE_UP,EYE_DOWN;

//Initialize ghost struct for orange ghost
struct ghost ghostOrange;

//Local pac-man struct to read pac_man's status
struct pacman pacmanStats_orange;

//Initialize ghost orange_directions & state
int orange_direction=GO_UP,orange_op_direction=GO_NOWHERE,orange_opflee_direction=GO_NOWHERE;
int ORANGE_GHOST_STATE = LOCKED;
int orangeFrightTime = 0;

void ghostOrangeMovementCalculation(){
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	const TickType_t Tick = 20;

	unsigned char collisionStatus = 0;
	//counter used for controlling ghost status
	int lockTime = 750;

	// GHOST ORANGE: ****************************************************************************************************************
	ghostOrange.x = 128;//starting position for ghost orange
	ghostOrange.y = 108;
	ghostOrange.speed = 1;//default speed of the ghost

	while(1){
		//Halt the action of ghost of it doesn't receive any signal from pacman
		if(xQueueReceive(pacmanStatusToOrangeQueue, &pacmanStats_orange, portMAX_DELAY)==pdTRUE){

			//set ghost's status based on signals from pac_man
			if(ORANGE_GHOST_STATE == LOCKED && lockTime == 0) ORANGE_GHOST_STATE = GET_OUT;
			else if(pacmanStats_orange.x==RESET){
				//reset signal received. ghost goes back to original position and reset status
				lockTime = 750;
				orangeFrightTime = 0;
				ghostOrange.x=128;
				ghostOrange.y=108;
				ORANGE_GHOST_STATE = LOCKED;
			}else if(ORANGE_GHOST_STATE==FRIGHT && abs(ghostOrange.x-pacmanStats_orange.x)<=4 && abs(ghostOrange.y-pacmanStats_orange.y)<=4){
				//Ghost is eaten
				orangeFrightTime = 0;
				ORANGE_GHOST_STATE = EATEN;
				collisionStatus = GHOST_EATEN;
				xQueueSend(collisionQueue, &collisionStatus, 0);
				xQueueSend(uartCollisionQueue, &collisionStatus, 0);
			}else if(ORANGE_GHOST_STATE==CHASE && abs(ghostOrange.x-pacmanStats_orange.x)<=2 && abs(ghostOrange.y-pacmanStats_orange.y)<=2){
				//Send death signal to pac_man
				collisionStatus = PAC_MAN_DIED;
				xQueueSend(collisionQueue, &collisionStatus, 0);
				xQueueSend(uartCollisionQueue, &collisionStatus, 0);
			}

			if(pacmanStats_orange.status==POWERED && (ORANGE_GHOST_STATE==CHASE || ORANGE_GHOST_STATE==FRIGHT)){
				orangeFrightTime = 1;
				ORANGE_GHOST_STATE = FRIGHT;
				orange_op_direction = GO_NOWHERE;
			}else if(orangeFrightTime == END && ORANGE_GHOST_STATE==FRIGHT){
				orangeFrightTime = 0;
				ORANGE_GHOST_STATE = CHASE;
				orange_opflee_direction = GO_NOWHERE;
			}

			//set ghost movement and orange_direction based on ghost state
			switch(ORANGE_GHOST_STATE){
				case LOCKED:
					lockTime -= 1;
					Orange_In_Box();
					break;
				case GET_OUT:
					Get_Out_Orange();
					if(ghostOrange.x == 112 && ghostOrange.y == 84) ORANGE_GHOST_STATE = CHASE;
					break;
				case CHASE:
					Orange_Chase();
					break;
				case FRIGHT:
					orangeFrightTime += 1;
					//execute ghost only half of the cycle, thus slowing down the speed
					if(orangeFrightTime%2==0)Orange_Frightened();
					break;
				case EATEN:
					Orange_Eaten();
					if(ghostOrange.x == 112 && ghostOrange.y == 108) ORANGE_GHOST_STATE = GET_OUT;
					break;
			}

			Draw_Orange();
		}
		vTaskDelayUntil(&xLastWakeTime, Tick);
	}
}

//function to calculate ghost's movements inside center box
void Orange_In_Box(){
	if(ghostOrange.y!=100 && orange_direction!=GO_DOWN)ghostOrange.y-=ghostOrange.speed;
	else if(ghostOrange.y!=116 && orange_direction!=GO_UP)ghostOrange.y+=ghostOrange.speed;
	if(ghostOrange.y==100) orange_direction = GO_DOWN;
	else if(ghostOrange.y==116) orange_direction = GO_UP;
}

//function to calculate ghost's movements getting out of the box
void Get_Out_Orange(){
	//center the ghost
	if(ghostOrange.x==112)ghostOrange.y-=1;
	else if(ghostOrange.y==108){
		if(ghostOrange.x!=112)ghostOrange.x-=1;
	}
	else if(ghostOrange.y<108) ghostOrange.y+=1;
	else if(ghostOrange.y>108) ghostOrange.y-=1;
}

//function to calculate movements to chase pac-man
void Orange_Chase(){
	int minIndex=0;
	// CALCULATION OF DISTANCES
	double up = pow((double) (ghostOrange.y - 4) - pacmanStats_orange.y, 2) + pow((double) ghostOrange.x - pacmanStats_orange.x, 2);
	double down = pow((double) (ghostOrange.y + 4) - pacmanStats_orange.y, 2) + pow((double) ghostOrange.x - pacmanStats_orange.x, 2);
	double right = pow((double) (ghostOrange.x + 4) - pacmanStats_orange.x, 2) + pow((double) ghostOrange.y - pacmanStats_orange.y, 2);
	double left = pow((double) (ghostOrange.x - 4) - pacmanStats_orange.x, 2) + pow((double) ghostOrange.y - pacmanStats_orange.y, 2);
	double distanceArray[4] = {left,right,up,down};

	//Compare distance of different orange_direction and find index of shortest path
	for(int i=0;i<4;i++){
		//if the orange_direction is impossible, increase the distance so the index will not be chosen
		if(!checkMovementGhost(ghostOrange.x,ghostOrange.y,i,0) || i==orange_op_direction){
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
				orange_op_direction = GO_LEFT;
				ghostOrange.x += ghostOrange.speed;
				if(ghostOrange.x>=224) ghostOrange.x=0;
			break;
		case GO_LEFT:
				orange_op_direction = GO_RIGHT;
				ghostOrange.x -= ghostOrange.speed;
				if(ghostOrange.x<=0) ghostOrange.x=224;
			break;
		case GO_UP:
				orange_op_direction = GO_DOWN;
				ghostOrange.y -= ghostOrange.speed;
			break;
		case GO_DOWN:
				orange_op_direction = GO_UP;
				ghostOrange.y += ghostOrange.speed;
			break;
		default:
			break;
	}
	orange_direction = minIndex;
}

//movement function when ghost is frightened, move away from pacman
void Orange_Frightened(){
	// CALCULATION OF DISTANCES
	double up = pow((double) (ghostOrange.y - 4) - pacmanStats_orange.y, 2) + pow((double) ghostOrange.x - pacmanStats_orange.x, 2);
	double down = pow((double) (ghostOrange.y + 4) - pacmanStats_orange.y, 2) + pow((double) ghostOrange.x - pacmanStats_orange.x, 2);
	double right = pow((double) (ghostOrange.x + 4) - pacmanStats_orange.x, 2) + pow((double) ghostOrange.y - pacmanStats_orange.y, 2);
	double left = pow((double) (ghostOrange.x - 4) - pacmanStats_orange.x, 2) + pow((double) ghostOrange.y - pacmanStats_orange.y, 2);
	double distanceArray[4] = {left,right,up,down};

	int maxIndex=0;
	//Compare distance of different orange_direction and find index of longest path
	for(int i=0;i<4;i++){
		//if the orange_direction is impossible, decrease the distance so the index will not be chosen
		if(!checkMovementGhost(ghostOrange.x,ghostOrange.y,i,0) || i==orange_opflee_direction){
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
				orange_opflee_direction = GO_LEFT;
				ghostOrange.x += ghostOrange.speed;
				if(ghostOrange.y == 108 && ghostOrange.x>=224) ghostOrange.x=0;
			break;
		case GO_LEFT:
				orange_opflee_direction = GO_RIGHT;
				ghostOrange.x -= ghostOrange.speed;
				if(ghostOrange.y == 108 && ghostOrange.x<=0) ghostOrange.x=224;
			break;
		case GO_UP:
				orange_opflee_direction = GO_DOWN;
				ghostOrange.y -= ghostOrange.speed;
			break;
		case GO_DOWN:
				orange_opflee_direction = GO_UP;
				ghostOrange.y += ghostOrange.speed;
			break;
		default:
			break;
	}
	orange_direction = maxIndex;
}

//movement function when ghost is eaten, flee back to center box
void Orange_Eaten(){
	int return_x=112,return_y=108;
	int minIndex=0;
	// CALCULATION OF DISTANCES
	double up = pow((double) (ghostOrange.y - 4) - return_y, 2) + pow((double) ghostOrange.x - return_x, 2);
	double down = pow((double) (ghostOrange.y + 4) - return_y, 2) + pow((double) ghostOrange.x - return_x, 2);
	double right = pow((double) (ghostOrange.x + 4) - return_x, 2) + pow((double) ghostOrange.y - return_y, 2);
	double left = pow((double) (ghostOrange.x - 4) - return_x, 2) + pow((double) ghostOrange.y - return_y, 2);
	double distanceArray[4] = {left,right,up,down};

	//Compare distance of different orange_direction and find index of shortest path
	for(int i=0;i<4;i++){
		//if the orange_direction is impossible, increase the distance so the index will not be chosen
		if(!checkMovementGhost(ghostOrange.x,ghostOrange.y,i,4) || i==orange_op_direction){
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
				orange_op_direction = GO_LEFT;
				ghostOrange.x += ghostOrange.speed;
				if(ghostOrange.y == 108 && ghostOrange.x>=224) ghostOrange.x=0;
			break;
		case GO_LEFT:
				orange_op_direction = GO_RIGHT;
				ghostOrange.x -= ghostOrange.speed;
				if(ghostOrange.y == 108 && ghostOrange.x<=0) ghostOrange.x=224;
			break;
		case GO_UP:
				orange_op_direction = GO_DOWN;
				ghostOrange.y -= ghostOrange.speed;
			break;
		case GO_DOWN:
				orange_op_direction = GO_UP;
				ghostOrange.y += ghostOrange.speed;
			break;
		default:
			break;
	}
	orange_direction = minIndex;
}

void Draw_Orange(){
	//draw the ghost based on orange_direction and state of the ghost
	if(ORANGE_GHOST_STATE == FRIGHT){
		if(orangeFrightTime > 475) gdispImageDraw(&WHITE,ghostOrange.x-7,ghostOrange.y-7,14,14,0,0);
		else if(orangeFrightTime > 450) gdispImageDraw(&FRIGHTENED,ghostOrange.x-7,ghostOrange.y-7,14,14,0,0);
		else if(orangeFrightTime > 425) gdispImageDraw(&WHITE,ghostOrange.x-7,ghostOrange.y-7,14,14,0,0);
		else if(orangeFrightTime > 400) gdispImageDraw(&FRIGHTENED,ghostOrange.x-7,ghostOrange.y-7,14,14,0,0);
		else if(orangeFrightTime > 375) gdispImageDraw(&WHITE,ghostOrange.x-7,ghostOrange.y-7,14,14,0,0);
		else if(orangeFrightTime > 400) gdispImageDraw(&FRIGHTENED,ghostOrange.x-7,ghostOrange.y-7,14,14,0,0);
		else if(orangeFrightTime > 375) gdispImageDraw(&WHITE,ghostOrange.x-7,ghostOrange.y-7,14,14,0,0);
		else if(orangeFrightTime > 350) gdispImageDraw(&FRIGHTENED,ghostOrange.x-7,ghostOrange.y-7,14,14,0,0);
		else if(orangeFrightTime > 325) gdispImageDraw(&WHITE,ghostOrange.x-7,ghostOrange.y-7,14,14,0,0);
		else if(orangeFrightTime > 300) gdispImageDraw(&FRIGHTENED,ghostOrange.x-7,ghostOrange.y-7,14,14,0,0);
		else if(orangeFrightTime > 275) gdispImageDraw(&WHITE,ghostOrange.x-7,ghostOrange.y-7,14,14,0,0);
		else gdispImageDraw(&FRIGHTENED,ghostOrange.x-7,ghostOrange.y-7,14,14,0,0);
	}else{
		switch(orange_direction){
			case GO_RIGHT:
					if(ORANGE_GHOST_STATE == EATEN)gdispImageDraw(&EYE_RIGHT,ghostOrange.x-7,ghostOrange.y-7,14,14,0,0);
					else gdispImageDraw(&ORANGE_RIGHT,ghostOrange.x-7,ghostOrange.y-7,14,14,0,0);
				break;
			case GO_LEFT:
					if(ORANGE_GHOST_STATE == EATEN)gdispImageDraw(&EYE_LEFT,ghostOrange.x-7,ghostOrange.y-7,14,14,0,0);
					else gdispImageDraw(&ORANGE_LEFT,ghostOrange.x-7,ghostOrange.y-7,14,14,0,0);
				break;
			case GO_UP:
					if(ORANGE_GHOST_STATE == EATEN)gdispImageDraw(&EYE_UP,ghostOrange.x-7,ghostOrange.y-7,14,14,0,0);
					else gdispImageDraw(&ORANGE_UP,ghostOrange.x-7,ghostOrange.y-7,14,14,0,0);
				break;
			case GO_DOWN:
					if(ORANGE_GHOST_STATE == EATEN)gdispImageDraw(&EYE_DOWN,ghostOrange.x-7,ghostOrange.y-7,14,14,0,0);
					else gdispImageDraw(&ORANGE_DOWN,ghostOrange.x-7,ghostOrange.y-7,14,14,0,0);
				break;
			default:
				break;
		}
	}
}
