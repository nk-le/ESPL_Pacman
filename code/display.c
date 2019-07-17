/*
 * display.c
 *This task will draw everything needed for the game play
 *
 *  Created on: Jun 18, 2019
 *      Author: lab_espl_stud11
 */
//#include "Map.h"
//#include "includes.h"
#include "Map.h"
#include "Demo.h"
#include "pac_man.h"

#define RIGHT_CURVE 						gdispDrawArc(x_cord,y_cord,4,90,180,Blue);
#define RIGHT_CURVE_SQUARE '1'

//Draw map for level 1
void drawMap(void * param){
	font_t font1; // Load font for ugfx
	font1 = gdispOpenFont("DejaVuSans24*");
	char c;
	struct pacman foodAte;

	xSemaphoreTake(food1->lock,portMAX_DELAY);
		if(xQueueReceive(pacmanStatusToMapQueue, &foodAte, 0)){
			if(foodAte.x == RESETFOOD) resetFoodMap();
			else if(foodAte.x == DRAW_CHERRY) food1->food[16][14]='c';
			else if(foodAte.x == DRAW_STRAWBERRY) food1->food[16][14]='s';
			else if(foodAte.x == DRAW_ORANGE) food1->food[16][14]='o';
			else food1->food[foodAte.y/8][foodAte.x/8] = '0';
		}
		int x_cord=8,y_cord=8;
		//using a loop to draw out the map
		for(int x=0;x<MAZE_LEN;x++){
			for (int y=0;y<MAZE_WID;y++){
				c = maze1[x][y];
				switch(c){
					case RIGHT_CURVE_SQUARE://upper left curve
						RIGHT_CURVE
						break;
					case '2'://upper right curve
						gdispDrawArc(x_cord-8,y_cord,4,0,90,Blue);
						break;
					case '3'://bottom left curve
						gdispDrawArc(x_cord,y_cord-8,4,180,270,Blue);
						break;
					case '4'://bottom right curve
						gdispDrawArc(x_cord-8,y_cord-8,4,270,0,Blue);
						break;
					case '-'://horizontal line
						gdispDrawLine(x_cord-8,y_cord-4,x_cord,y_cord-4,Blue);
						break;
					case 'd'://the small door in the middle
						gdispDrawLine(x_cord-8,y_cord-4,x_cord,y_cord-4,Maroon);
						break;
					case '|'://vertical lines
						gdispDrawLine(x_cord-4,y_cord-8,x_cord-4,y_cord,Blue);
						break;
					}
				if(food1->food[x][y]=='.') gdispFillArea(x_cord-5,y_cord-5,2,2,Pink);
				else if(food1->food[x][y]=='p')gdispFillCircle(x_cord-4,y_cord-4,3,Pink);
				else if(food1->food[x][y]=='c')gdispImageDraw(&CHERRY,105,125,14,14,0,0);
				else if(food1->food[x][y]=='s')gdispImageDraw(&STRAW,105,125,14,14,0,0);
				else if(food1->food[x][y]=='o')gdispImageDraw(&ORANGE,105,125,14,14,0,0);
				x_cord+=8;
			}
			x_cord=8;//reset x coordinates
			y_cord+=8;//increase y coordinate, move to next line
		}
		//semaphore give to pac-man, telling him map is done updating
		xSemaphoreGive(food1->lock);
}

void resetFoodMap(){
	unsigned char i,j;
	for(i = 0; i< 30; i++){
		for(j = 0; j <28; j++){
			food1->food[i][j] = food[i][j];
		}
	}
}


