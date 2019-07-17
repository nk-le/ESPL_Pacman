/*
 * ghostBlue.c
 *
 *  Created on: Jun 28, 2019
 *      Author: lab_espl_stud11
 */
//#include "includes.h"
#include "ghost.h"
//#include "gfx.h"
#include "Button.h"
#include "Demo.h"
#include "pac_man.h"
//#include "romfs_files.h"

//gdispImage BLUE_UP,BLUE_DOWN,BLUE_LEFT,BLUE_RIGHT,FRIGHTENED,
//				EYE_LEFT,EYE_RIGHT,EYE_UP,EYE_DOWN;

//Initialize ghost struct for blue ghost
struct ghost ghostBlue;

//Local pac-man struct to read pac_man's status
struct pacman pacmanStats_blue;

//Initialize ghost blue_directions & state
int blue_direction=GO_UP,blue_op_direction=GO_NOWHERE,blue_opflee_direction=GO_NOWHERE;
int BLUE_GHOST_STATE = LOCKED;
int blueFrightTime = 0;

void ghostBlueMovementCalculation(){
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	const TickType_t Tick = 20;

	unsigned char collisionStatus = 0;
	//counter used for controlling ghost status
	int lockTime = 250;

	// GHOST BLUE: ****************************************************************************************************************
	ghostBlue.x = 96;//starting position for ghost blue
	ghostBlue.y = 108;
	ghostBlue.speed = 1;//default speed of the ghost

	while(1){
		//Halt the action of ghost of it doesn't receive any signal from pacman
		if(xQueueReceive(pacmanStatusToBlueQueue, &pacmanStats_blue, portMAX_DELAY)==pdTRUE){

			//set ghost's status based on signals from pac_man
			if(BLUE_GHOST_STATE==LOCKED && lockTime == 0) BLUE_GHOST_STATE = GET_OUT;
			else if(pacmanStats_blue.x==RESET){
				//reset signal received. ghost goes back to original position and reset status
				lockTime = 250;
				blueFrightTime = 0;
				ghostBlue.x=96;
				ghostBlue.y=108;
				BLUE_GHOST_STATE = LOCKED;
			}else if(BLUE_GHOST_STATE==FRIGHT && abs(ghostBlue.x-pacmanStats_blue.x)<=4 && abs(ghostBlue.y-pacmanStats_blue.y)<=4){
				//Ghost is eaten
				blueFrightTime = 0;
				BLUE_GHOST_STATE = EATEN;
				collisionStatus = GHOST_EATEN;
				xQueueSend(collisionQueue, &collisionStatus, 0);
				xQueueSend(uartCollisionQueue, &collisionStatus, 0);
			}else if(BLUE_GHOST_STATE==CHASE && abs(ghostBlue.x-pacmanStats_blue.x)<=2 && abs(ghostBlue.y-pacmanStats_blue.y)<=2){
				//Send death signal to pac_man
				collisionStatus = PAC_MAN_DIED;
				xQueueSend(collisionQueue, &collisionStatus, 0);
				xQueueSend(uartCollisionQueue, &collisionStatus, 0);
			}

			if(pacmanStats_blue.status==POWERED && (BLUE_GHOST_STATE==CHASE || BLUE_GHOST_STATE==FRIGHT)){
				blueFrightTime = 1;
				BLUE_GHOST_STATE = FRIGHT;
				blue_op_direction = GO_NOWHERE;
			}else if(blueFrightTime == END && BLUE_GHOST_STATE==FRIGHT){
				blueFrightTime = 0;
				BLUE_GHOST_STATE = CHASE;
				blue_opflee_direction = GO_NOWHERE;
			}

			//set ghost movement and blue_direction based on ghost state
			switch(BLUE_GHOST_STATE){
				case LOCKED:
					lockTime -= 1;
					Blue_In_Box();
					break;
				case GET_OUT:
					Get_Out_Blue();
					if(ghostBlue.x == 112 && ghostBlue.y == 84) BLUE_GHOST_STATE = CHASE;
					break;
				case CHASE:
					Blue_Chase();
					break;
				case FRIGHT:
					blueFrightTime += 1;
					//execute ghost only half of the cycle, thus slowing down the speed
					if(blueFrightTime%2==0)Blue_Frightened();
					break;
				case EATEN:
					Blue_Eaten();
					if(ghostBlue.x == 112 && ghostBlue.y == 108) BLUE_GHOST_STATE = GET_OUT;
					break;
			}

			Draw_Blue();
		}

		vTaskDelayUntil(&xLastWakeTime, Tick);
	}
}

//function to calculate ghost's movements inside center box
void Blue_In_Box(){
	if(ghostBlue.y!=100 && blue_direction!=GO_DOWN)ghostBlue.y-=ghostBlue.speed;
	else if(ghostBlue.y!=116 && blue_direction!=GO_UP)ghostBlue.y+=ghostBlue.speed;
	if(ghostBlue.y==100) blue_direction = GO_DOWN;
	else if(ghostBlue.y==116) blue_direction = GO_UP;
}

//function to calculate ghost's movements getting out of the box
void Get_Out_Blue(){
	//center the ghost
	if(ghostBlue.x==112)ghostBlue.y-=1;
	else if(ghostBlue.y==108){
		if(ghostBlue.x!=112)ghostBlue.x+=1;
	}
	else if(ghostBlue.y<108) ghostBlue.y+=1;		//uxHighWaterMarkGameOver = uxTaskGetStackHighWaterMark( NULL );
	else if(ghostBlue.y>108) ghostBlue.y-=1;
}

//function to calculate movements to chase pac-man
void Blue_Chase(){
	int minIndex=0;
	// CALCULATION OF DISTANCES
	double up = pow((double) (ghostBlue.y - 4) - pacmanStats_blue.y, 2) + pow((double) ghostBlue.x - pacmanStats_blue.x, 2);
	double down = pow((double) (ghostBlue.y + 4) - pacmanStats_blue.y, 2) + pow((double) ghostBlue.x - pacmanStats_blue.x, 2);
	double right = pow((double) (ghostBlue.x + 4) - pacmanStats_blue.x, 2) + pow((double) ghostBlue.y - pacmanStats_blue.y, 2);
	double left = pow((double) (ghostBlue.x - 4) - pacmanStats_blue.x, 2) + pow((double) ghostBlue.y - pacmanStats_blue.y, 2);
	double distanceArray[4] = {left,right,up,down};

	//Compare distance of different blue_direction and find index of shortest path
	for(int i=0;i<4;i++){
		//if the blue_direction is impossible, increase the distance so the index will not be chosen
		if(!checkMovementGhost(ghostBlue.x,ghostBlue.y,i,0) || i==blue_op_direction){
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
				blue_op_direction = GO_LEFT;
				ghostBlue.x += ghostBlue.speed;
				if(ghostBlue.y == 108 && ghostBlue.x>=224) ghostBlue.x=0;
			break;
		case GO_LEFT:
				blue_op_direction = GO_RIGHT;
				ghostBlue.x -= ghostBlue.speed;
				if(ghostBlue.y == 108 && ghostBlue.x<=0) ghostBlue.x=224;
			break;
		case GO_UP:
				blue_op_direction = GO_DOWN;
				ghostBlue.y -= ghostBlue.speed;
			break;
		case GO_DOWN:
				blue_op_direction = GO_UP;
				ghostBlue.y += ghostBlue.speed;
			break;
		default:
			break;
	}
	blue_direction = minIndex;
}

//movement function when ghost is frightened, move away from pacman
void Blue_Frightened(){
	// CALCULATION OF DISTANCES
	double up = pow((double) (ghostBlue.y - 4) - pacmanStats_blue.y, 2) + pow((double) ghostBlue.x - pacmanStats_blue.x, 2);
	double down = pow((double) (ghostBlue.y + 4) - pacmanStats_blue.y, 2) + pow((double) ghostBlue.x - pacmanStats_blue.x, 2);
	double right = pow((double) (ghostBlue.x + 4) - pacmanStats_blue.x, 2) + pow((double) ghostBlue.y - pacmanStats_blue.y, 2);
	double left = pow((double) (ghostBlue.x - 4) - pacmanStats_blue.x, 2) + pow((double) ghostBlue.y - pacmanStats_blue.y, 2);
	double distanceArray[4] = {left,right,up,down};

	int maxIndex=0;
	//Compare distance of different blue_direction and find index of longest path
	for(int i=0;i<4;i++){
		//if the blue_direction is impossible, decrease the distance so the index will not be chosen
		if(!checkMovementGhost(ghostBlue.x,ghostBlue.y,i,0) || i==blue_opflee_direction){
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
				blue_opflee_direction = GO_LEFT;
				ghostBlue.x += ghostBlue.speed;
				if(ghostBlue.y == 108 && ghostBlue.x>=224) ghostBlue.x=0;
			break;
		case GO_LEFT:
				blue_opflee_direction = GO_RIGHT;
				ghostBlue.x -= ghostBlue.speed;
				if(ghostBlue.y == 108 && ghostBlue.x<=0) ghostBlue.x=224;
			break;
		case GO_UP:
				blue_opflee_direction = GO_DOWN;
				ghostBlue.y -= ghostBlue.speed;
			break;
		case GO_DOWN:
				blue_opflee_direction = GO_UP;
				ghostBlue.y += ghostBlue.speed;
			break;
		default:
			break;
	}
	blue_direction = maxIndex;
}

//movement function when ghost is eaten, flee back to center box
void Blue_Eaten(){
	int return_x=112,return_y=108;
	int minIndex=0;
	// CALCULATION OF DISTANCES
	double up = pow((double) (ghostBlue.y - 4) - return_y, 2) + pow((double) ghostBlue.x - return_x, 2);
	double down = pow((double) (ghostBlue.y + 4) - return_y, 2) + pow((double) ghostBlue.x - return_x, 2);
	double right = pow((double) (ghostBlue.x + 4) - return_x, 2) + pow((double) ghostBlue.y - return_y, 2);
	double left = pow((double) (ghostBlue.x - 4) - return_x, 2) + pow((double) ghostBlue.y - return_y, 2);
	double distanceArray[4] = {left,right,up,down};

	//Compare distance of different blue_direction and find index of shortest path
	for(int i=0;i<4;i++){
		//if the blue_direction is impossible, increase the distance so the index will not be chosen
		if(!checkMovementGhost(ghostBlue.x,ghostBlue.y,i,4) || i==blue_op_direction){
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
				blue_op_direction = GO_LEFT;
				ghostBlue.x += ghostBlue.speed;
				if(ghostBlue.y == 108 && ghostBlue.x>=224) ghostBlue.x=0;
			break;
		case GO_LEFT:
				blue_op_direction = GO_RIGHT;
				ghostBlue.x -= ghostBlue.speed;
				if(ghostBlue.y == 108 && ghostBlue.x<=0) ghostBlue.x=224;
			break;
		case GO_UP:
				blue_op_direction = GO_DOWN;
				ghostBlue.y -= ghostBlue.speed;
			break;
		case GO_DOWN:
				blue_op_direction = GO_UP;
				ghostBlue.y += ghostBlue.speed;
			break;
		default:
			break;
	}
	blue_direction = minIndex;
}

void Draw_Blue(){
	//draw the ghost based on blue_direction and state of the ghost
	if(BLUE_GHOST_STATE == FRIGHT){
		if(blueFrightTime > 475) gdispImageDraw(&WHITE,ghostBlue.x-7,ghostBlue.y-7,14,14,0,0);
		else if(blueFrightTime > 450) gdispImageDraw(&FRIGHTENED,ghostBlue.x-7,ghostBlue.y-7,14,14,0,0);
		else if(blueFrightTime > 425) gdispImageDraw(&WHITE,ghostBlue.x-7,ghostBlue.y-7,14,14,0,0);
		else if(blueFrightTime > 400) gdispImageDraw(&FRIGHTENED,ghostBlue.x-7,ghostBlue.y-7,14,14,0,0);
		else if(blueFrightTime > 375) gdispImageDraw(&WHITE,ghostBlue.x-7,ghostBlue.y-7,14,14,0,0);
		else if(blueFrightTime > 400) gdispImageDraw(&FRIGHTENED,ghostBlue.x-7,ghostBlue.y-7,14,14,0,0);
		else if(blueFrightTime > 375) gdispImageDraw(&WHITE,ghostBlue.x-7,ghostBlue.y-7,14,14,0,0);
		else if(blueFrightTime > 350) gdispImageDraw(&FRIGHTENED,ghostBlue.x-7,ghostBlue.y-7,14,14,0,0);
		else if(blueFrightTime > 325) gdispImageDraw(&WHITE,ghostBlue.x-7,ghostBlue.y-7,14,14,0,0);
		else if(blueFrightTime > 300) gdispImageDraw(&FRIGHTENED,ghostBlue.x-7,ghostBlue.y-7,14,14,0,0);
		else if(blueFrightTime > 275) gdispImageDraw(&WHITE,ghostBlue.x-7,ghostBlue.y-7,14,14,0,0);
		else gdispImageDraw(&FRIGHTENED,ghostBlue.x-7,ghostBlue.y-7,14,14,0,0);
	}else{
		switch(blue_direction){
			case GO_RIGHT:
					if(BLUE_GHOST_STATE == EATEN)gdispImageDraw(&EYE_RIGHT,ghostBlue.x-7,ghostBlue.y-7,14,14,0,0);
					else gdispImageDraw(&BLUE_RIGHT,ghostBlue.x-7,ghostBlue.y-7,14,14,0,0);
				break;
			case GO_LEFT:
					if(BLUE_GHOST_STATE == EATEN)gdispImageDraw(&EYE_LEFT,ghostBlue.x-7,ghostBlue.y-7,14,14,0,0);
					else gdispImageDraw(&BLUE_LEFT,ghostBlue.x-7,ghostBlue.y-7,14,14,0,0);
				break;
			case GO_UP:
					if(BLUE_GHOST_STATE == EATEN)gdispImageDraw(&EYE_UP,ghostBlue.x-7,ghostBlue.y-7,14,14,0,0);
					else gdispImageDraw(&BLUE_UP,ghostBlue.x-7,ghostBlue.y-7,14,14,0,0);
				break;
			case GO_DOWN:
					if(BLUE_GHOST_STATE == EATEN)gdispImageDraw(&EYE_DOWN,ghostBlue.x-7,ghostBlue.y-7,14,14,0,0);
					else gdispImageDraw(&BLUE_DOWN,ghostBlue.x-7,ghostBlue.y-7,14,14,0,0);
				break;
			default:
				break;
		}
		}
}
