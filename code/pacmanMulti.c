/*
 * pacmanMulti.c
 *
 *  Created on: Jul 11, 2019
 *      Author: lab_espl_stud10
 */

/*
 *This task will calculate the pac_man coordinate and post the draw pac_man request
 *
 *  Created on: Jun 28, 2019
 *      Author: lab_espl_stud11
 */
//#include "includes.h"
#include "Map.h"
#include "pac_man.h"
#include "Button.h"
#include "gfx.h"
#include "queue.h"
#include "Demo.h"
#include "../Libraries/ugfx/src/gdisp/gdisp_image.h"

#define GO_LEFT 0
#define GO_RIGHT 1
#define GO_UP 2
#define GO_DOWN 3

#define singleMode 1
#define multiMode 2

void pacmanMulti(void * params){
	font_t font1; // Load font for ugfx
	font1 = gdispOpenFont("DejaVuSans24*");
	char str[10];
	struct buttons buttonStatus; // joystick queue input buffer
	unsigned char checkDie = 0;

	int figure_x=112,figure_y=180, powercount = 0;//starting position of pacman figure
	char c;

	int direction=4,figureDirection=0,score=0,life=4;
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	const TickType_t tickFramerate = 20;
	struct pacman pacmanStats = {{0}};
	struct pacman uartPacman;
	pacmanStats.status = 0;
	unsigned char collision,restart=0;
	unsigned char playMode = multiMode;


	while(1){
		if (xQueueReceive(uartPacmanQueue, &uartPacman, portMAX_DELAY) == pdTRUE){ // This will need pacman: x, y, direction
		int figure_x = uartPacman.x, figure_y = uartPacman.y, direction = uartPacman.direction, figure_map_x=(figure_x+4)%8,figure_map_y=(figure_y+4)%8;

		switch(direction){
			case GO_RIGHT:
				c = food1->food[figure_y/8][(figure_x-4)/8+1];
				if( c == '.' || c == '0' || c == 'p')figure_x+=1;
				else if(figure_x==224)figure_x=0;
				else if(figure_y == 108 && figure_x >= 220)figure_x+=1;
				figureDirection = GO_RIGHT;
				break;
			case GO_LEFT:
				c = food1->food[figure_y/8][(figure_x+3)/8-1];
				//( c == '.' || c == '0' || c == 'p')figure_x-=1;
				if(figure_x==-3)figure_x=224;
				else if(figure_y == 108 && figure_x<=4)figure_x-=1;
				else if( c == '.' || c == '0' || c == 'p')figure_x-=1;
				figureDirection = GO_LEFT;
				break;
			case GO_DOWN:
				c = food1->food[(figure_y-4)/8+1][figure_x/8];
				if( c == '.' || c == '0' || c == 'p')figure_y+=1;
				figureDirection = GO_DOWN;
				break;
			case GO_UP:
				c = food1->food[(figure_y+3)/8-1][figure_x/8];
				if( c == '.' || c == '0' || c == 'p')figure_y-=1;
				figureDirection = GO_UP;
				break;
			default: break;
		}

		//set the counter for power mode and update pac_man status
		if(powercount>0) powercount+=1;
		if(powercount==500) {
			powercount = 0;
			pacmanStats.status = 0;

		}

		//draw figure based on action & orientation
		c = food1->food[figure_y/8][figure_x/8];
		if(c =='.'){//if the covered coordinate has food
			food1->food[figure_y/8][figure_x/8] = '0';//consume the food
			score+=10;//increase score by 10
			gdispFillCircle(figure_x,figure_y,6,Yellow);
		}else if(c=='p'){//if the covered coordinate is a pellet
			food1->food[figure_y/8][figure_x/8] = '0';//consume the pellet
			powercount+=1;
			pacmanStats.status = 1;
			//send semaphore to make ghost change color and movement speed
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
				default: gdispFillCircle(112,180,6,Yellow);
			}
		}

		xQueueReceive(restartQueue, &restart, 0);
		xQueueReceive(collisionQueue, &collision, 0);
		if(restart){
			//when pac man died
			restart = 0;
			figure_x = 112;
			figure_y = 180;
			pacmanStats.x = 250;
			pacmanStats.status = 0;
			powercount = 0;
			life = 4;
			score = 0;
			//RESET MAP
				}
		else{
			if(collision == 1 && pacmanStats.status == 0)
			{
				//when pac man died
				figure_x = 112;
				figure_y = 180;
				pacmanStats.x = 250;
				life-=1;
				collision = 0;
			}
			else if(collision == 2){
				score+=200;
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

		//draw life and score
		sprintf(str,"%d",score);
		gdispDrawString(255, 60, str, font1, White);
		sprintf(str,"life: %d",life);
		gdispDrawString(255, 70, str, font1, White);

		xQueueSend(pacmanStatusToRedQueue, &pacmanStats, 0);
		xQueueSend(pacmanStatusToBlueQueue, &pacmanStats, 0);
		xQueueSend(pacmanStatusToOrangeQueue, &pacmanStats, 0);
		xQueueSend(pacmanStatusToPinkQueue, &pacmanStats, 0);
		xQueueSend(pacmanStatusToMultiGhostQueue, &pacmanStats, 0);

		// PACMAN DIES, ACTIVATE GAME OVER STATE
		if(life < 0){
		life = 0;
		checkDie = 1;
		xQueueSend(pacmanDieQueue, &checkDie, 0);
		xQueueSend(scoreQueue, &score, 0);
		}
		else checkDie = 0;

		}

		//sendPosition(buttonStatus); // Ham nay hoat dong hoan toan binh thuong khi duoc su dung de gui button status qua Uart
		vTaskDelayUntil(&xLastWakeTime, tickFramerate);
	}
}





