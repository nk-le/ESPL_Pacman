/*
 *  ghostMulti.c
 *
 *  Created on: Jun 28, 2019
 *      Author: lab_espl_stud11
 */

#include "ghost.h"
#include "gfx.h"
#include "Button.h"
#include "Demo.h"
#include "pac_man.h"
#include "romfs_files.h"
#include "Map.h"
#include "pac_man.h"
#include "Button.h"
#include "queue.h"
#include "StateMachine.h"

//Initialize ghost struct for multi ghost
struct ghost ghostMulti;
struct buttons buttonStatus;
//Local pac-man struct to read pac_man's status
struct pacman pacmanStats_multi;
struct pacman foodAte;

//Initialize ghost multi_directions & state
int multi_direction=GO_UP,multi_op_direction=GO_NOWHERE,multi_opflee_direction=GO_NOWHERE;
int MULTI_GHOST_STATE = CHASE;
int multiFrightTime = 0;
unsigned char initX, initY;
unsigned char str[100];
int lockTime,checkDie=0;
unsigned char saveLockTime;

void  ghostMultiMovementCalculation(){
	font_t font1; // Load font for ugfx
	font1 = gdispOpenFont("DejaVuSans24*");
	char str[10];//string buffer
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	const TickType_t Tick = 20;

	//place holder to read the map
	char c;
	//declare food timer and appearence time
	int fruitTime = 0,fruitCount=2, currentLevel=1,score=0,life=4,oneUp=0,dotCount=1,
			collision=0;

	// GHOST MULTI: ****************************************************************************************************************
	while(1){
		//Halt the action of ghost of it doesn't receive any signal from pacman
		if(xQueueReceive(buttonGhostQueue, &buttonStatus, portMAX_DELAY) == pdTRUE){
			xQueueReceive(buttonGhostQueue, &buttonStatus, 0);
			xQueueReceive(pacmanStatusToMultiGhostQueue, &pacmanStats_multi, 0);

			//ghost stuff:
			if(pacmanStats_multi.uartCheck == PAUSING){
				unsigned char next_state_signal = statePauseMenuMulti;
				xQueueSend(stateChangeSignalQueue, &next_state_signal, 100);
			}else{
				if(xQueueReceive(ghostMultiQueue, & ghostMulti, 0)==pdTRUE){
					initX =  ghostMulti.x;
					initY =  ghostMulti.y;
					if(initX == BLUE_INIT_X){
						saveLockTime = LOCK_TIME_BLUE;
						MULTI_GHOST_STATE = LOCKED;
					}else if(initX == ORANGE_INIT_X){
						saveLockTime = LOCK_TIME_ORANGE;
						MULTI_GHOST_STATE = LOCKED;
					}else if(initX == PINK_INIT_X && initY == PINK_INIT_Y){
						saveLockTime = LOCK_TIME_PINK;
						MULTI_GHOST_STATE = LOCKED;
					}
					lockTime = saveLockTime;
				}

				//set ghost's status based on signals from pac_man
				if(MULTI_GHOST_STATE == LOCKED && lockTime == 0 && initY != RED_INIT_Y) MULTI_GHOST_STATE = GET_OUT;
				if(pacmanStats_multi.x==RESET){
					//reset signal received. ghost goes back to original position and reset status
					if(initX == 112 && initY == 84) MULTI_GHOST_STATE = CHASE;
					else MULTI_GHOST_STATE = LOCKED ;
					lockTime = saveLockTime;
					multiFrightTime = 0;
					ghostMulti.x=initX;
					ghostMulti.y=initY;
				}else{

					if(MULTI_GHOST_STATE==FRIGHT && abs( ghostMulti.x-pacmanStats_multi.x)<=4 && abs( ghostMulti.y-pacmanStats_multi.y)<=4){
						//Ghost is eaten
						MULTI_GHOST_STATE = EATEN;
						//collisionStatus = GHOST_EATEN;
						multiFrightTime = 0;
						collision = GHOST_EATEN;
						//xQueueSend(collisionQueue, &collisionStatus, 100);
					}else if(MULTI_GHOST_STATE==CHASE && abs( ghostMulti.x-pacmanStats_multi.x)<=2 && abs( ghostMulti.y-pacmanStats_multi.y)<=2){
						//Set death signal to pac_man
						collision = PAC_MAN_DIED;
					}

					if(pacmanStats_multi.status==POWERED && (MULTI_GHOST_STATE==CHASE || MULTI_GHOST_STATE==FRIGHT)){
						multiFrightTime = 1;
						MULTI_GHOST_STATE = FRIGHT;
						multi_op_direction = GO_NOWHERE;
					}else if(multiFrightTime == END && MULTI_GHOST_STATE==FRIGHT){
						multiFrightTime = 0;
						MULTI_GHOST_STATE = CHASE;
						multi_opflee_direction = GO_NOWHERE;
					}

					//set ghost movement and multi_direction based on ghost state
					switch(MULTI_GHOST_STATE){
						case LOCKED:
							lockTime -= 1;
							Multi_In_Box();
							break;
						case GET_OUT:
							Get_Out_Multi();
							if( ghostMulti.x == 112 &&  ghostMulti.y == 84) MULTI_GHOST_STATE = CHASE;
							break;
						case CHASE:
							Multi_Chase();
							break;
						case FRIGHT:
							multiFrightTime += 1;
							//execute ghost only half of the cycle, thus slowing down the speed
							if (multiFrightTime%2==0)Multi_Frightened();
							break;
						case EATEN:
							Multi_Eaten();
							if( ghostMulti.x == 112 &&  ghostMulti.y == 108) MULTI_GHOST_STATE = GET_OUT;
							break;
					}

					//draw ghost on screen immediately after calculation is done
					Draw_Multi();


					//pac-man stuff:

					//start timer for fruit
					if(fruitTime<501)fruitTime += 1;

					//when 10 seconds is up, put fruit on the map
					if(fruitTime == 500 && fruitCount >0){
						fruitCount-=1;//decrease the remaining food appearance time by one
						if(currentLevel == 1)foodAte.x = DRAW_CHERRY;
						else if(currentLevel == 2) foodAte.x = DRAW_STRAWBERRY;
						else if(currentLevel == 3)foodAte.x = DRAW_ORANGE;
						xQueueSend(pacmanStatusToMapQueue, &foodAte, 0);//tell map to draw fruit
					}

					//wait for the semaphore from map before reading from map
					xSemaphoreTake(food1->lock,portMAX_DELAY);

					//send update map signal to the drawMap function
					c = food1->food[pacmanStats_multi.y/8][pacmanStats_multi.x/8];
					if(c!='0'){
						switch(c){
							case '.'://if pac-man is on a dot, eat the dot
								foodAte.x = pacmanStats_multi.x;
								foodAte.y = pacmanStats_multi.y;
								xQueueSend(pacmanStatusToMapQueue, &foodAte, 0);//tell map to update
								//food1->food[pacmanStats_multi.y/8][pacmanStats_multi.x/8]='0';
								score+=10;//increase score by 10
								oneUp+=10;
								dotCount+=1;
								gdispFillCircle(pacmanStats_multi.x,pacmanStats_multi.y,6,Yellow);
								break;
							case 'p'://if pac-man is on a power pellet, eat the power pellet
								foodAte.x = pacmanStats_multi.x;
								foodAte.y = pacmanStats_multi.y;
								//if(!settings.power){
									//food1->food[pacmanStats_multi.y/8][pacmanStats_multi.x/8] = '0';
									xQueueSend(pacmanStatusToMapQueue, &foodAte, 0);//tell map to update
								//}
								score+=50;
								oneUp+=50;
								//pacmanStats.status = POWERED;
								gdispFillCircle(pacmanStats_multi.x,pacmanStats_multi.y,6,Yellow);
								break;
							case 'c'://pac-man eat cherry
								foodAte.x = pacmanStats_multi.x;
								foodAte.y = pacmanStats_multi.y;
								xQueueSend(pacmanStatusToMapQueue, &foodAte, 0);//tell map to update
								//food1->food[pacmanStats_multi.y/8][pacmanStats_multi.x/8] = '0';
								score+=100;//increase score by 10
								oneUp+=100;
								fruitTime = 0;//reset fruit timer
								gdispFillCircle(pacmanStats_multi.x,pacmanStats_multi.y,6,Yellow);
								break;
							case 's'://pac-man eat strawberry
								foodAte.x = pacmanStats_multi.x;
								foodAte.y = pacmanStats_multi.y;
								//food1->food[pacmanStats_multi.y/8][pacmanStats_multi.x/8] = '0';
								xQueueSend(pacmanStatusToMapQueue, &foodAte, 0);//tell map to update
								score+=300;//increase score by 10
								oneUp+=300;
								fruitTime = 0;//reset fruit timer
								gdispFillCircle(pacmanStats_multi.x,pacmanStats_multi.y,6,Yellow);
								break;
							case 'o'://pac-man eat orange
								foodAte.x = pacmanStats_multi.x;
								foodAte.y = pacmanStats_multi.y;
								//food1->food[pacmanStats_multi.y/8][pacmanStats_multi.x/8] = '0';
								xQueueSend(pacmanStatusToMapQueue, &foodAte, 0);//tell map to update
								score+=500;//increase score by 10
								oneUp+=500;
								fruitTime = 0;//reset fruit timer
								gdispFillCircle(pacmanStats_multi.x,pacmanStats_multi.y,6,Yellow);
								break;
						}
					}
					//GIVE semaphore
					xSemaphoreGive(food1->lock);
				}
				//draw pac-man based on direction
				switch(pacmanStats_multi.direction){
					case GO_RIGHT:
						gdispFillArc(pacmanStats_multi.x,pacmanStats_multi.y,6,50,310,Yellow);
						break;
					case GO_LEFT:
						gdispFillArc(pacmanStats_multi.x,pacmanStats_multi.y,6,230,130,Yellow);
						break;
					case GO_DOWN:
						gdispFillArc(pacmanStats_multi.x,pacmanStats_multi.y,6,320,220,Yellow);
						break;
					case GO_UP:
						gdispFillArc(pacmanStats_multi.x,pacmanStats_multi.y,6,140,40,Yellow);
						break;
				}

				//add an additional life for every 3000 points
				if(oneUp>=3000){
					life+=1;
					oneUp=0;
				}

				//if all the dots on the map is eaten
				if(dotCount==282){
					if(currentLevel<3){
						currentLevel+=1;
						foodAte.x = RESETFOOD;
						xQueueSend(pacmanStatusToMapQueue, &foodAte, 0);//tell map to reset
						//resetFoodMap();
						dotCount=1;
						//direction = GO_NOWHERE;
						fruitTime = 0;
						fruitCount = 2;
					}else if(currentLevel==3){
						//if it is level 3 and all dots are eaten, send victory signal
						checkDie = 2;
						xQueueSend(pacmanDieQueue, &checkDie, 100);
						xQueueSend(scoreQueue, &score, 100);
					}
				}

				//Check pac-man's collision with the Ghosts
				if(collision ==0) xQueueReceive(uartCollisionQueue, &collision, 0);

				if(collision == PAC_MAN_DIED){
					//when pac man died
					life-=1; //decrease life if no cheat activated
					collision = 0;
				}
				else if(collision == GHOST_EATEN){
					score+=200;
					oneUp+=200;
					collision = 0;
				}

				// PACMAN DIES, ACTIVATE GAME OVER STATE
				if(life == 0){
					life = 0;
					checkDie = 1;
					xQueueSend(pacmanDieQueue, &checkDie, 100);
					xQueueSend(scoreMultiQueue, &score, 100);
				}
				else checkDie = 0;

				//draw life and score
				sprintf(str,"%d",score);
				gdispDrawString(255, 60, str, font1, White);
				sprintf(str,"life: %d",life);
				gdispDrawString(255, 70, str, font1, White);
				sprintf(str,"Level: %d",currentLevel);
				gdispDrawString(255, 30, str, font1, White);
				switch(currentLevel){
					case 1:
						gdispImageDraw(&CHERRY,250,200,14,14,0,0);
						break;
					case 2:
						gdispImageDraw(&STRAW,250,200,14,14,0,0);
						break;
					case 3:
						gdispImageDraw(&ORANGE,250,200,14,14,0,0);
						break;
				}

				//Update for Multiplayer Mode
				 ghostMulti.state = MULTI_GHOST_STATE;
				 ghostMulti.direction = multi_direction;
				 ghostMulti.uartCheck = PLAYING;
				 sendStatusGhost( ghostMulti);
			}
			vTaskDelayUntil(&xLastWakeTime, Tick);
	}
}
}

//function to calculate ghost's movements inside center box
void Multi_In_Box(){
	if( ghostMulti.y!=100 && multi_direction!=GO_DOWN) ghostMulti.y-= ghostMulti.speed;
	else if( ghostMulti.y!=116 && multi_direction!=GO_UP) ghostMulti.y+= ghostMulti.speed;
	if( ghostMulti.y==100) multi_direction = GO_DOWN;
	else if( ghostMulti.y==116) multi_direction = GO_UP;
}

//function to calculate ghost's movements getting out of the box
void Get_Out_Multi(){
	//center the ghost
	if( ghostMulti.x==112) ghostMulti.y-=1;
	else if( ghostMulti.y==108){
		if( ghostMulti.x!=112 && initX == BLUE_INIT_X) ghostMulti.x+=1;
		else if(ghostMulti.x!=112 && initX == ORANGE_INIT_X) ghostMulti.x-=1;
	}
	else if( ghostMulti.y<108)  ghostMulti.y+=1;
	else if( ghostMulti.y>108)  ghostMulti.y-=1;
}

//function to calculate movements to chase pac-man
void Multi_Chase(){
	int joy_x=buttonStatus.joystick.x, joy_y = buttonStatus.joystick.y, figure_map_x=( ghostMulti.x+4)%8,figure_map_y=( ghostMulti.y+4)%8;

	if(joy_x >= 240 && figure_map_y==0)  ghostMulti.direction = GO_RIGHT;
	else if(joy_x<=10 && figure_map_y==0)  ghostMulti.direction = GO_LEFT;
	else if(joy_y >= 240 && figure_map_x==0)  ghostMulti.direction = GO_DOWN;
	else if(joy_y <= 10 && figure_map_x==0)  ghostMulti.direction = GO_UP;

	unsigned char c;
	switch( ghostMulti.direction){
		case GO_RIGHT:
			c = maze1[ ghostMulti.y/8][( ghostMulti.x-4)/8+1];
			if( c == '.') ghostMulti.x+=1;
			else if( ghostMulti.x>=224) ghostMulti.x=0;
			else if( ghostMulti.y == 108 &&  ghostMulti.x >= 220) ghostMulti.x+=1;
			 ghostMulti.direction = GO_RIGHT;
			break;
		case GO_LEFT:
			c = maze1[ ghostMulti.y/8][( ghostMulti.x+3)/8-1];
			if( ghostMulti.x <= 0) ghostMulti.x=224;
			else if( ghostMulti.y == 108 &&  ghostMulti.x<=4) ghostMulti.x-=1;
			else if( c == '.') ghostMulti.x-=1;
			 ghostMulti.direction = GO_LEFT;
			break;
		case GO_DOWN:
			c = maze1[( ghostMulti.y-4)/8+1][ ghostMulti.x/8];
			if( c == '.') ghostMulti.y+=1;
			 ghostMulti.direction = GO_DOWN;
			break;
		case GO_UP:
			c = maze1[( ghostMulti.y+3)/8-1][ ghostMulti.x/8];
			if( c == '.') ghostMulti.y-=1;
			 ghostMulti.direction = GO_UP;
			break;
		default:
			ghostMulti.direction = GO_UP;
			break;
	}

	//set the next coordinate to go
	multi_direction =  ghostMulti.direction;
}

//movement function when ghost is frightened, move away from pacman
void Multi_Frightened(){
	int joy_x=buttonStatus.joystick.x, joy_y = buttonStatus.joystick.y, figure_map_x=( ghostMulti.x+4)%8,figure_map_y=( ghostMulti.y+4)%8;

	if(joy_x >= 240 && figure_map_y==0)  ghostMulti.direction = GO_RIGHT;
	else if(joy_x<=10 && figure_map_y==0)  ghostMulti.direction = GO_LEFT;
	else if(joy_y >= 240 && figure_map_x==0)  ghostMulti.direction = GO_DOWN;
	else if(joy_y <= 10 && figure_map_x==0)  ghostMulti.direction = GO_UP;

	unsigned char c;
	switch( ghostMulti.direction){
		case GO_RIGHT:
			c = maze1[ ghostMulti.y/8][( ghostMulti.x-4)/8+1];
			if( c == '.') ghostMulti.x+=1;
			else if( ghostMulti.x==224) ghostMulti.x=0;
			else if( ghostMulti.y == 108 &&  ghostMulti.x >= 220) ghostMulti.x+=1;
			 ghostMulti.direction = GO_RIGHT;
			break;
		case GO_LEFT:
			c = maze1[ ghostMulti.y/8][( ghostMulti.x+3)/8-1];
			if( ghostMulti.x==-3) ghostMulti.x=224;
			else if( ghostMulti.y == 108 &&  ghostMulti.x<=4) ghostMulti.x-=1;
			else if( c == '.') ghostMulti.x-=1;
			 ghostMulti.direction = GO_LEFT;
			break;
		case GO_DOWN:
			c = maze1[( ghostMulti.y-4)/8+1][ ghostMulti.x/8];
			if( c == '.') ghostMulti.y+=1;
			 ghostMulti.direction = GO_DOWN;
			break;
		case GO_UP:
			c = maze1[( ghostMulti.y+3)/8-1][ ghostMulti.x/8];
			if( c == '.') ghostMulti.y-=1;
			 ghostMulti.direction = GO_UP;
			break;
		default:
			ghostMulti.direction = GO_UP;
			break;
	}

	//set the next coordinate to go
	multi_direction =  ghostMulti.direction;
}

//movement function when ghost is eaten, flee back to center box
void Multi_Eaten(){
	int return_x=112,return_y=108;
	int minIndex=0;
	// CALCULATION OF DISTANCES
	double up = pow((double) ( ghostMulti.y - 4) - return_y, 2) + pow((double)  ghostMulti.x - return_x, 2);
	double down = pow((double) ( ghostMulti.y + 4) - return_y, 2) + pow((double)  ghostMulti.x - return_x, 2);
	double right = pow((double) ( ghostMulti.x + 4) - return_x, 2) + pow((double)  ghostMulti.y - return_y, 2);
	double left = pow((double) ( ghostMulti.x - 4) - return_x, 2) + pow((double)  ghostMulti.y - return_y, 2);
	double distanceArray[4] = {left,right,up,down};

	//Compare distance of different multi_direction and find index of shortest path
	for(int i=0;i<4;i++){
		//if the multi_direction is impossible, increase the distance so the index will not be chosen
		if(!checkMovementGhost( ghostMulti.x, ghostMulti.y,i,4) || i==multi_op_direction){
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
				multi_op_direction = GO_LEFT;
				 ghostMulti.x +=  ghostMulti.speed;
				if( ghostMulti.y == 108 &&  ghostMulti.x>=224)  ghostMulti.x=0;
			break;
		case GO_LEFT:
				multi_op_direction = GO_RIGHT;
				 ghostMulti.x -=  ghostMulti.speed;
				if( ghostMulti.y == 108 &&  ghostMulti.x<=0)  ghostMulti.x=224;
			break;
		case GO_UP:
				multi_op_direction = GO_DOWN;
				 ghostMulti.y -=  ghostMulti.speed;
			break;
		case GO_DOWN:
				multi_op_direction = GO_UP;
				 ghostMulti.y +=  ghostMulti.speed;
			break;
		default:
			break;
	}
	multi_direction = minIndex;
}

void Draw_Multi(){
	//draw the ghost based on multi_direction and state of the ghost
	if(MULTI_GHOST_STATE == FRIGHT){
		if(multiFrightTime > 475) gdispImageDraw(&WHITE, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0);
		else if(multiFrightTime > 450) gdispImageDraw(&FRIGHTENED, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0);
		else if(multiFrightTime > 425) gdispImageDraw(&WHITE, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0);
		else if(multiFrightTime > 400) gdispImageDraw(&FRIGHTENED, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0);
		else if(multiFrightTime > 375) gdispImageDraw(&WHITE, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0);
		else if(multiFrightTime > 400) gdispImageDraw(&FRIGHTENED, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0);
		else if(multiFrightTime > 375) gdispImageDraw(&WHITE, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0);
		else if(multiFrightTime > 350) gdispImageDraw(&FRIGHTENED, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0);
		else if(multiFrightTime > 325) gdispImageDraw(&WHITE, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0);
		else if(multiFrightTime > 300) gdispImageDraw(&FRIGHTENED, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0);
		else if(multiFrightTime > 275) gdispImageDraw(&WHITE, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0);
		else gdispImageDraw(&FRIGHTENED, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0);
	}else{
		switch(multi_direction){
			case GO_RIGHT:
				if(MULTI_GHOST_STATE == EATEN)gdispImageDraw(&EYE_RIGHT, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0);
				else{
					if(initX == RED_INIT_X && initY == RED_INIT_Y)gdispImageDraw(&RED_RIGHT, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0); // Red
					if(initX == BLUE_INIT_X)gdispImageDraw(&BLUE_RIGHT, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0); // Blue
					if(initX == ORANGE_INIT_X)gdispImageDraw(&ORANGE_RIGHT, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0); // Orange
					if(initX == PINK_INIT_X && initY == PINK_INIT_Y)gdispImageDraw(&PINK_RIGHT, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0); // Pink
				}
				break;
			case GO_LEFT:
				if(MULTI_GHOST_STATE == EATEN)gdispImageDraw(&EYE_LEFT, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0);
				else{
					if(initX == RED_INIT_X && initY == RED_INIT_Y)gdispImageDraw(&RED_LEFT, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0);
					if(initX == BLUE_INIT_X)gdispImageDraw(&BLUE_LEFT, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0);
					if(initX == ORANGE_INIT_X)gdispImageDraw(&ORANGE_LEFT, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0);
					if(initX == PINK_INIT_X && initY == PINK_INIT_Y)gdispImageDraw(&PINK_LEFT, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0);
				}
				break;
			case GO_UP:
				if(MULTI_GHOST_STATE == EATEN)gdispImageDraw(&EYE_UP, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0);
				else{
					if(initX == RED_INIT_X && initY == RED_INIT_Y)gdispImageDraw(&RED_UP, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0);
					if(initX == BLUE_INIT_X)gdispImageDraw(&BLUE_UP, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0);
					if(initX == ORANGE_INIT_X)gdispImageDraw(&ORANGE_UP, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0);
					if(initX == PINK_INIT_X && initY == PINK_INIT_Y)gdispImageDraw(&PINK_UP, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0);
				}
				break;
			case GO_DOWN:
				if(MULTI_GHOST_STATE == EATEN)gdispImageDraw(&EYE_DOWN, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0);
				else{
					if(initX == RED_INIT_X && initY == RED_INIT_Y)gdispImageDraw(&RED_DOWN, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0);
					if(initX == BLUE_INIT_X)gdispImageDraw(&BLUE_DOWN, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0);
					if(initX == ORANGE_INIT_X)gdispImageDraw(&ORANGE_DOWN, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0);
					if(initX == PINK_INIT_X && initY == PINK_INIT_Y)gdispImageDraw(&PINK_DOWN, ghostMulti.x-7, ghostMulti.y-7,14,14,0,0);
				}
				break;
			default:
				break;
		}
	}
}
