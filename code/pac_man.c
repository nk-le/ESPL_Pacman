/*
 *This task will calculate the pac_man coordinate and post the draw pac_man request
 *
 *  Created on: Jun 28, 2019
 *      Author: lab_espl_stud11
 */
//#include "includes.h"
#include "Map.h"
#include "pac_man.h"
#include "ghost.h"
#include "Button.h"
//#include "gfx.h"
#include "queue.h"
#include "Demo.h"
#include "romfs_files.h"

void pac_man(void * params){
	font_t font1; // Load font for ugfx
	font1 = gdispOpenFont("DejaVuSans24*");
	char str[10];
	struct buttons buttonStatus; // joystick queue input buffer
	struct cheat settings;//struct to contain cheat settings
	settings.power=0;
	settings.life=0;
	unsigned char checkDie = 0;
	unsigned char playMode = SINGLE_MODE; // set default play mode to single player

	int figure_x=112,figure_y=180,dotCount=0,currentLevel=1;//starting position of pacman figure
	char c;
	//declare food timer and appearence time
	int fruitTime = 0,fruitCount=2;
	int direction=GO_NOWHERE,figureDirection=0,score=0,life=4,oneUp=0;
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	const TickType_t tickFramerate = 20;
	struct pacman pacmanStats = {{0}};
	struct pacman foodAte;
	pacmanStats.status = NORMAL;
	unsigned char collision,restart=0;


	while(1){
			//vairables used to create frame rates
			unsigned char strTick[100];
			TickType_t lastTime;
			lastTime = xTaskGetTickCount();

			//halt pac-man's action until it receives something from the button queue
			if (xQueueReceive(buttonPacmanQueue, &buttonStatus, portMAX_DELAY) == pdTRUE){

				//check restart signal
				xQueueReceive(restartQueue, &restart, 0);
				//reset all game status if a restart signal is received
				if(restart){
					settings.life = 0;
					settings.power = 0;
					restart = 0;
					figure_x = 112;//starting coordinates of pac-man
					figure_y = 180;
					pacmanStats.x = RESET;
					pacmanStats.status = NORMAL;
					life = 4;
					score = 0;
					oneUp = 0;
					dotCount = 0;
					direction = GO_NOWHERE;
					currentLevel = 1;
					fruitTime = 0;
					fruitCount = 2;
				}else {
					//check cheat settings
					if(xQueueReceive(cheatQueue, &settings,0) == pdTRUE){
						if(settings.life){
							life = 99;
						}
						if(settings.score > 0) {
							score = settings.score;
							settings.score = 0;
						}
						if(settings.level!=0) currentLevel = settings.level+1;
					}

					//start timer for fruit
					if(fruitTime<501)fruitTime += 1;

					//when 10 seconds is up, put fruit on the map
					if(fruitTime == 500 && fruitCount >0){
						fruitCount-=1;//decrease the remaining food appearance time by one
						if(currentLevel == 1) foodAte.x = DRAW_CHERRY;
						else if(currentLevel == 2) foodAte.x = DRAW_STRAWBERRY;
						else if(currentLevel == 3)foodAte.x = DRAW_ORANGE;
						xQueueSend(pacmanStatusToMapQueue, &foodAte, 0);//tell map to draw fruit
					}

					//read direction based on joystick values
					int joy_x=buttonStatus.joystick.x,joy_y=buttonStatus.joystick.y
							,figure_map_x=(figure_x+4)%8,figure_map_y=(figure_y+4)%8;

					if(joy_x >= 240 && figure_map_y==0) direction = GO_RIGHT;
					else if(joy_x<=10 && figure_map_y==0) direction = GO_LEFT;
					else if(joy_y >= 240 && figure_map_x==0) direction = GO_DOWN;
					else if(joy_y <= 10 && figure_map_x==0) direction = GO_UP;

					//wait for the semaphore from map before reading from map
					xSemaphoreTake(food1->lock,portMAX_DELAY);

					//Read input decision and let pac-man moves based on map layout
					switch(direction){
						case GO_RIGHT:
							c = food1->food[figure_y/8][(figure_x-4)/8+1];
							if( c == '.' || c == '0' || c == 'p' || c=='c' || c=='s' || c=='o')figure_x+=1;
							else if(figure_x==224)figure_x=0;
							else if(figure_y == 108 && figure_x >= 220)figure_x+=1;
							figureDirection = GO_RIGHT;
							break;
						case GO_LEFT:
							c = food1->food[figure_y/8][(figure_x+3)/8-1];
							//( c == '.' || c == '0' || c == 'p')figure_x-=1;
							if(figure_x==-3)figure_x=224;
							else if(figure_y == 108 && figure_x<=4)figure_x-=1;
							else if( c == '.' || c == '0' || c == 'p' || c=='c' || c=='s' || c=='o')figure_x-=1;
							figureDirection = GO_LEFT;
							break;
						case GO_DOWN:
							c = food1->food[(figure_y-4)/8+1][figure_x/8];
							if( c == '.' || c == '0' || c == 'p' || c=='c' || c=='s' || c=='o')figure_y+=1;
							figureDirection = GO_DOWN;
							break;
						case GO_UP:
							c = food1->food[(figure_y+3)/8-1][figure_x/8];
							if( c == '.' || c == '0' || c == 'p' || c=='c' || c=='s' || c=='o')figure_y-=1;
							figureDirection = GO_UP;
							break;
					}

					//draw figure based on action & orientation
					c = food1->food[figure_y/8][figure_x/8];
					if(c!='0'){
						switch(c){
							case '.'://if pac-man is on a dot, eat the dot
								foodAte.x = figure_x;
								foodAte.y = figure_y;
								xQueueSend(pacmanStatusToMapQueue, &foodAte, 0);//tell map to update
								//food1->food[figure_y/8][figure_x/8]='0';
								score+=10;//increase score by 10
								oneUp+=10;
								dotCount+=1;
								gdispFillCircle(figure_x,figure_y,6,Yellow);
								break;
							case 'p'://if pac-man is on a power pellet, eat the power pellet
								foodAte.x = figure_x;
								foodAte.y = figure_y;
								if(!settings.power){
									//food1->food[figure_y/8][figure_x/8] = '0';
									xQueueSend(pacmanStatusToMapQueue, &foodAte, 0);//tell map to update
								}
								score+=50;
								oneUp+=50;
								pacmanStats.status = POWERED;
								gdispFillCircle(figure_x,figure_y,6,Yellow);
								break;
							case 'c'://pac-man eat cherry
								foodAte.x = figure_x;
								foodAte.y = figure_y;
								xQueueSend(pacmanStatusToMapQueue, &foodAte, 0);//tell map to update
								//food1->food[figure_y/8][figure_x/8] = '0';
								score+=100;//increase score by 10
								oneUp+=100;
								fruitTime = 0;//reset fruit timer
								gdispFillCircle(figure_x,figure_y,6,Yellow);
								break;
							case 's'://pac-man eat strawberry
								foodAte.x = figure_x;
								foodAte.y = figure_y;
								//food1->food[figure_y/8][figure_x/8] = '0';
								xQueueSend(pacmanStatusToMapQueue, &foodAte, 0);//tell map to update
								score+=300;//increase score by 10
								oneUp+=300;
								fruitTime = 0;//reset fruit timer
								gdispFillCircle(figure_x,figure_y,6,Yellow);
								break;
							case 'o'://pac-man eat orange
								foodAte.x = figure_x;
								foodAte.y = figure_y;
								//food1->food[figure_y/8][figure_x/8] = '0';
								xQueueSend(pacmanStatusToMapQueue, &foodAte, 0);//tell map to update
								score+=500;//increase score by 10
								oneUp+=500;
								fruitTime = 0;//reset fruit timer
								gdispFillCircle(figure_x,figure_y,6,Yellow);
								break;
						}
					}else{//normal actions
						//choose which direction the Pac-man is facing
						switch(figureDirection){
							case GO_RIGHT:
								gdispFillArc(figure_x,figure_y,6,50,310,Yellow);
								break;
							case GO_LEFT:
								gdispFillArc(figure_x,figure_y,6,230,130,Yellow);
								break;
							case GO_DOWN:
								gdispFillArc(figure_x,figure_y,6,320,220,Yellow);
								break;
							case GO_UP:
								gdispFillArc(figure_x,figure_y,6,140,40,Yellow);
								break;
						}
					}

					//GIVE semaphore
					xSemaphoreGive(food1->lock);

					//Check pac-man's collision with the Ghosts
					xQueueReceive(collisionQueue, &collision, 0);
					if(collision == PAC_MAN_DIED){
						//when pac man died
						figure_x = 112;
						figure_y = 180;
						pacmanStats.x = 250;
						if(!settings.life) life-=1; //decrease life if no cheat activated
						collision = 0;
						direction = GO_NOWHERE;
					}
					else if(collision == GHOST_EATEN){
						score+=200;
						oneUp+=200;
						pacmanStats.x = figure_x;
						pacmanStats.y = figure_y;
						collision = 0;
					}
					else
					{
						pacmanStats.x = figure_x;
						pacmanStats.y = figure_y;
					}

			}

			//add an additional life for every 3000 points
			if(oneUp>=3000 && !settings.life){
				life+=1;
				oneUp=0;
			}

			//if all the dots on the map is eaten
			if(dotCount==282){
				if(currentLevel<3){
					currentLevel+=1;
					pacmanStats.x = 250;
					pacmanStats.status = NORMAL;
					foodAte.x = RESETFOOD;
					xQueueSend(pacmanStatusToMapQueue, &foodAte, 0);//tell map to reset
					//resetFoodMap();
					figure_x = 112;
					figure_y = 180;
					dotCount=0;
					direction = GO_NOWHERE;
					fruitTime = 0;
					fruitCount = 2;
				}else if(currentLevel==3){
					//if it is level 3 and all dots are eaten, send victory signal
					checkDie = 2;
					xQueueSend(pacmanDieQueue, &checkDie, 100);
					xQueueSend(scoreQueue, &score, 100);
				}
			}



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

			//update for other Tasks
			//pacmanStats.life = life;
			pacmanStats.uartCheck = PLAYING;
			pacmanStats.direction = figureDirection;
			xQueueSend(pacmanStatusToRedQueue, &pacmanStats, 0);
			xQueueSend(pacmanStatusToBlueQueue, &pacmanStats, 0);
			xQueueSend(pacmanStatusToOrangeQueue, &pacmanStats,0);
			xQueueSend(pacmanStatusToPinkQueue, &pacmanStats, 0);
			xQueueSend(pacmanStatusToMultiGhostQueue, &pacmanStats, 0);

			xQueueReceive(playingModeQueue, &playMode, 0);
			// Only for 2 player Mode
			if(playMode != SINGLE_MODE){
				sendStatusPacman(pacmanStats);
			}

			//reset pac-man's status
			pacmanStats.status = 0;

			// PACMAN DIES, ACTIVATE GAME OVER STATE
			if(life == 0){
				life = 0;
				checkDie = 1;
				xQueueSend(pacmanDieQueue, &checkDie, 100);
				if(playMode != SINGLE_MODE){
					xQueueSend(scoreMultiQueue, &score, 100);
				}else xQueueSend(scoreQueue, &score, 100);
			}
			else checkDie = 0;

			//Display the frame-rate of current iteration
			TickType_t exTime = xTaskGetTickCount() - lastTime;
			unsigned char frame;
			xQueueReceive(frameQueue, &frame, 0);
			unsigned char freq = 1000/frame;
			sprintf(strTick, "Freq: %d", freq);
			gdispDrawString(255, 220, strTick, font1, White);

		}
		vTaskDelayUntil(&xLastWakeTime, tickFramerate);
	}
}
