/*
 * ghostPink.c
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

//gdispImage PINK_UP,PINK_DOWN,PINK_LEFT,PINK_RIGHT,FRIGHTENED,
//				EYE_LEFT,EYE_RIGHT,EYE_UP,EYE_DOWN;

//Initialize ghost struct for pink ghost
struct ghost ghostPink;

//Local pac-man struct to read pac_man's status
struct pacman pacmanStats_pink;

//Initialize ghost pink_directions & state
int pink_direction=GO_UP,pink_op_direction=GO_NOWHERE,pink_opflee_direction=GO_NOWHERE;
int PINK_GHOST_STATE = LOCKED;
int pinkFrightTime = 0;

void ghostPinkMovementCalculation(){
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	const TickType_t Tick = 20;

	unsigned char collisionStatus = 0;
	//counters to control ghost status
	int lockTime = 500;

	// GHOST PINK: ****************************************************************************************************************
	ghostPink.x = 112;//starting position for ghost pink
	ghostPink.y = 108;
	ghostPink.speed = 1;//default speed of the ghost

	while(1){
		//Halt the action of ghost of it doesn't receive any signal from pacman
		if(xQueueReceive(pacmanStatusToPinkQueue, &pacmanStats_pink, portMAX_DELAY)==pdTRUE){

			//set ghost's status based on signals from pac_man
			if(PINK_GHOST_STATE==LOCKED && lockTime == 0) PINK_GHOST_STATE = GET_OUT;
			else if(pacmanStats_pink.x==RESET){
				//reset signal received. ghost goes back to original position and reset status
				lockTime = 500;
				pinkFrightTime = 0;
				ghostPink.x=112;
				ghostPink.y=108;
				PINK_GHOST_STATE = LOCKED;
			}else if(PINK_GHOST_STATE==FRIGHT && abs(ghostPink.x-pacmanStats_pink.x)<=4 && abs(ghostPink.y-pacmanStats_pink.y)<=4){
				//Ghost is eaten
				pinkFrightTime = 0;
				PINK_GHOST_STATE = EATEN;
				collisionStatus = GHOST_EATEN;
				xQueueSend(collisionQueue, &collisionStatus, 0);
				xQueueSend(uartCollisionQueue, &collisionStatus, 0);
			}else if(PINK_GHOST_STATE==CHASE && abs(ghostPink.x-pacmanStats_pink.x)<=2 && abs(ghostPink.y-pacmanStats_pink.y)<=2){
				//Send death signal to pac_man
				collisionStatus = PAC_MAN_DIED;
				xQueueSend(collisionQueue, &collisionStatus, 0);
				xQueueSend(uartCollisionQueue, &collisionStatus, 0);
			}

			if(pacmanStats_pink.status==POWERED && (PINK_GHOST_STATE==CHASE || PINK_GHOST_STATE==FRIGHT )){
				//if pac-man ate a power pellet
				pinkFrightTime = 1;
				PINK_GHOST_STATE = FRIGHT;
				pink_op_direction = GO_NOWHERE;
			}else if(pinkFrightTime == END && PINK_GHOST_STATE==FRIGHT){
				pinkFrightTime = 0;
				PINK_GHOST_STATE = CHASE;
				pink_opflee_direction = GO_NOWHERE;
			}

			//set ghost movement and pink_direction based on ghost state
			switch(PINK_GHOST_STATE){
				case LOCKED:
					lockTime -= 1;
					Pink_In_Box();
					break;
				case GET_OUT:
					Get_Out_Pink();
					if(ghostPink.x == 112 && ghostPink.y == 84) PINK_GHOST_STATE = CHASE;
					break;
				case CHASE:
					Pink_Chase();
					break;
				case FRIGHT:
					pinkFrightTime += 1;
					//execute ghost only half of the cycle, thus slowing down the speed
					if(pinkFrightTime%2==0)Pink_Frightened();
					break;
				case EATEN:
					Pink_Eaten();
					if(ghostPink.x == 112 && ghostPink.y == 108) PINK_GHOST_STATE = GET_OUT;
					break;
			}

			Draw_Pink();
		}
		vTaskDelayUntil(&xLastWakeTime, Tick);
	}
}

//function to calculate ghost's movements inside center box
void Pink_In_Box(){
	if(ghostPink.y!=100 && pink_direction!=GO_DOWN)ghostPink.y-=ghostPink.speed;
	else if(ghostPink.y!=116 && pink_direction!=GO_UP)ghostPink.y+=ghostPink.speed;
	if(ghostPink.y==100) pink_direction = GO_DOWN;
	else if(ghostPink.y==116) pink_direction = GO_UP;
}

//function to calculate ghost's movements getting out of the box
void Get_Out_Pink(){
	//center the ghost
	if(ghostPink.x==112)ghostPink.y-=1;
	else if(ghostPink.y==108){
		if(ghostPink.x!=112)ghostPink.x+=1;
	}
	else if(ghostPink.y<108) ghostPink.y+=1;
	else if(ghostPink.y>108) ghostPink.y-=1;
}

//function to calculate movements to chase pac-man
void Pink_Chase(){
	int minIndex=0;
	// CALCULATION OF DISTANCES
	double up = pow((double) (ghostPink.y - 4) - pacmanStats_pink.y, 2) + pow((double) ghostPink.x - pacmanStats_pink.x, 2);
	double down = pow((double) (ghostPink.y + 4) - pacmanStats_pink.y, 2) + pow((double) ghostPink.x - pacmanStats_pink.x, 2);
	double right = pow((double) (ghostPink.x + 4) - pacmanStats_pink.x, 2) + pow((double) ghostPink.y - pacmanStats_pink.y, 2);
	double left = pow((double) (ghostPink.x - 4) - pacmanStats_pink.x, 2) + pow((double) ghostPink.y - pacmanStats_pink.y, 2);
	double distanceArray[4] = {left,right,up,down};

	//Compare distance of different pink_direction and find index of shortest path
	for(int i=0;i<4;i++){
		//if the pink_direction is impossible, increase the distance so the index will not be chosen
		if(!checkMovementGhost(ghostPink.x,ghostPink.y,i,0) || i==pink_op_direction){
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
				pink_op_direction = GO_LEFT;
				ghostPink.x += ghostPink.speed;
				if(ghostPink.x>=224) ghostPink.x=0;
			break;
		case GO_LEFT:
				pink_op_direction = GO_RIGHT;
				ghostPink.x -= ghostPink.speed;
				if(ghostPink.x<=0) ghostPink.x=224;
			break;
		case GO_UP:
				pink_op_direction = GO_DOWN;
				ghostPink.y -= ghostPink.speed;
			break;
		case GO_DOWN:
				pink_op_direction = GO_UP;
				ghostPink.y += ghostPink.speed;
			break;
		default:
			break;
	}
	pink_direction = minIndex;
}

//movement function when ghost is frightened, move away from pacman
void Pink_Frightened(){
	// CALCULATION OF DISTANCES
	double up = pow((double) (ghostPink.y - 4) - pacmanStats_pink.y, 2) + pow((double) ghostPink.x - pacmanStats_pink.x, 2);
	double down = pow((double) (ghostPink.y + 4) - pacmanStats_pink.y, 2) + pow((double) ghostPink.x - pacmanStats_pink.x, 2);
	double right = pow((double) (ghostPink.x + 4) - pacmanStats_pink.x, 2) + pow((double) ghostPink.y - pacmanStats_pink.y, 2);
	double left = pow((double) (ghostPink.x - 4) - pacmanStats_pink.x, 2) + pow((double) ghostPink.y - pacmanStats_pink.y, 2);
	double distanceArray[4] = {left,right,up,down};

	int maxIndex=0;
	//Compare distance of different pink_direction and find index of longest path
	for(int i=0;i<4;i++){
		//if the pink_direction is impossible, decrease the distance so the index will not be chosen
		if(!checkMovementGhost(ghostPink.x,ghostPink.y,i,0) || i==pink_opflee_direction){
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
				pink_opflee_direction = GO_LEFT;
				ghostPink.x += ghostPink.speed;
				if(ghostPink.y == 108 && ghostPink.x>=224) ghostPink.x=0;
			break;
		case GO_LEFT:
				pink_opflee_direction = GO_RIGHT;
				ghostPink.x -= ghostPink.speed;
				if(ghostPink.y == 108 && ghostPink.x<=0) ghostPink.x=224;
			break;
		case GO_UP:
				pink_opflee_direction = GO_DOWN;
				ghostPink.y -= ghostPink.speed;
			break;
		case GO_DOWN:
				pink_opflee_direction = GO_UP;
				ghostPink.y += ghostPink.speed;
			break;
		default:
			break;
	}
	pink_direction = maxIndex;
}

//movement function when ghost is eaten, flee back to center box
void Pink_Eaten(){
	int return_x=112,return_y=108;
	int minIndex=0;
	// CALCULATION OF DISTANCES
	double up = pow((double) (ghostPink.y - 4) - return_y, 2) + pow((double) ghostPink.x - return_x, 2);
	double down = pow((double) (ghostPink.y + 4) - return_y, 2) + pow((double) ghostPink.x - return_x, 2);
	double right = pow((double) (ghostPink.x + 4) - return_x, 2) + pow((double) ghostPink.y - return_y, 2);
	double left = pow((double) (ghostPink.x - 4) - return_x, 2) + pow((double) ghostPink.y - return_y, 2);
	double distanceArray[4] = {left,right,up,down};

	//Compare distance of different pink_direction and find index of shortest path
	for(int i=0;i<4;i++){
		//if the pink_direction is impossible, increase the distance so the index will not be chosen
		if(!checkMovementGhost(ghostPink.x,ghostPink.y,i,4) || i==pink_op_direction){
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
				pink_op_direction = GO_LEFT;
				ghostPink.x += ghostPink.speed;
				if(ghostPink.y == 108 && ghostPink.x>=224) ghostPink.x=0;
			break;
		case GO_LEFT:
				pink_op_direction = GO_RIGHT;
				ghostPink.x -= ghostPink.speed;
				if(ghostPink.y == 108 && ghostPink.x<=0) ghostPink.x=224;
			break;
		case GO_UP:
				pink_op_direction = GO_DOWN;
				ghostPink.y -= ghostPink.speed;
			break;
		case GO_DOWN:
				pink_op_direction = GO_UP;
				ghostPink.y += ghostPink.speed;
			break;
		default:
			break;
	}
	pink_direction = minIndex;
}

void Draw_Pink(){
	//draw the ghost based on pink_direction and state of the ghost
	if(PINK_GHOST_STATE == FRIGHT){
		if(pinkFrightTime > 475) gdispImageDraw(&WHITE,ghostPink.x-7,ghostPink.y-7,14,14,0,0);
		else if(pinkFrightTime > 450) gdispImageDraw(&FRIGHTENED,ghostPink.x-7,ghostPink.y-7,14,14,0,0);
		else if(pinkFrightTime > 425) gdispImageDraw(&WHITE,ghostPink.x-7,ghostPink.y-7,14,14,0,0);
		else if(pinkFrightTime > 400) gdispImageDraw(&FRIGHTENED,ghostPink.x-7,ghostPink.y-7,14,14,0,0);
		else if(pinkFrightTime > 375) gdispImageDraw(&WHITE,ghostPink.x-7,ghostPink.y-7,14,14,0,0);
		else if(pinkFrightTime > 400) gdispImageDraw(&FRIGHTENED,ghostPink.x-7,ghostPink.y-7,14,14,0,0);
		else if(pinkFrightTime > 375) gdispImageDraw(&WHITE,ghostPink.x-7,ghostPink.y-7,14,14,0,0);
		else if(pinkFrightTime > 350) gdispImageDraw(&FRIGHTENED,ghostPink.x-7,ghostPink.y-7,14,14,0,0);
		else if(pinkFrightTime > 325) gdispImageDraw(&WHITE,ghostPink.x-7,ghostPink.y-7,14,14,0,0);
		else if(pinkFrightTime > 300) gdispImageDraw(&FRIGHTENED,ghostPink.x-7,ghostPink.y-7,14,14,0,0);
		else if(pinkFrightTime > 275) gdispImageDraw(&WHITE,ghostPink.x-7,ghostPink.y-7,14,14,0,0);
		else gdispImageDraw(&FRIGHTENED,ghostPink.x-7,ghostPink.y-7,14,14,0,0);
	}else{
		switch(pink_direction){
			case GO_RIGHT:
					if(PINK_GHOST_STATE == EATEN)gdispImageDraw(&EYE_RIGHT,ghostPink.x-7,ghostPink.y-7,14,14,0,0);
					else gdispImageDraw(&PINK_RIGHT,ghostPink.x-7,ghostPink.y-7,14,14,0,0);
				break;
			case GO_LEFT:
					if(PINK_GHOST_STATE == EATEN)gdispImageDraw(&EYE_LEFT,ghostPink.x-7,ghostPink.y-7,14,14,0,0);
					else gdispImageDraw(&PINK_LEFT,ghostPink.x-7,ghostPink.y-7,14,14,0,0);
				break;
			case GO_UP:
					if(PINK_GHOST_STATE == EATEN)gdispImageDraw(&EYE_UP,ghostPink.x-7,ghostPink.y-7,14,14,0,0);
					else gdispImageDraw(&PINK_UP,ghostPink.x-7,ghostPink.y-7,14,14,0,0);
				break;
			case GO_DOWN:
					if(PINK_GHOST_STATE == EATEN)gdispImageDraw(&EYE_DOWN,ghostPink.x-7,ghostPink.y-7,14,14,0,0);
					else gdispImageDraw(&PINK_DOWN,ghostPink.x-7,ghostPink.y-7,14,14,0,0);
				break;
			default:
				break;
		}
	}
}
