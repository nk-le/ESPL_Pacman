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
#include "../Libraries/ugfx/src/gdisp/gdisp_image.h"
#include "StateMachine.h"


#define GO_LEFT 0
#define GO_RIGHT 1
#define GO_UP 2
#define GO_DOWN 3

#define pacmanPlayer 1
#define ghostRed 2
#define ghostBlue 3
#define ghostOrange 4
#define ghostPink 5

//Update the color, update the start position
//update from Host, receive the button Queue from UART and make ghost movement here

void ghostMultiReceive(){
	font_t font1; // Load font for ugfx
	font1 = gdispOpenFont("DejaVuSans24*");

	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	const TickType_t tickFramerate = 20;

	char str[10];
	char str2[100] = {"WAITING FOR GHOST PLAYER"};
	struct buttons buttonStatus; // joystick queue input buffer
	struct multiplayer updatePlayer = {{ 0 }}; // joystick queue input buffer
	unsigned char checkDie = 0, fright = 0, eaten = 0;
	int MULTI_GHOST_STATE = CHASE;
	int multiFrightTime = 0;

	int figure_x=52,figure_y=36, powercount = 0;//starting position of pacman figure
	char c;

	int direction=5,figureDirection=0,score=0,life=4;

	struct pacman pacmanStats = {{0}};
	struct ghost ghostStatus;

	unsigned char collisionStatus ,restart=0;

	while(1){
		// Con Ghost nay nhan Ghost tu Uart va Pacman cua chinh board minh de dua ra conclusion
		if (xQueueReceive(ghostStatusQueue, &ghostStatus, portMAX_DELAY) == pdTRUE){
			if(ghostStatus.uartCheck == PLAYING){
				xQueueReceive(pacmanStatusToMultiGhostQueue, &pacmanStats, 0);
				xQueueReceive(ghostMultiReceiveQueue, &updatePlayer, 0); // Player 2 will be another board
				MULTI_GHOST_STATE = ghostStatus.state;

				// CHECK COLLISION
				if(pacmanStats.x == 250)
				{
					eaten = 0;
				}
				// Return back, ghost kills Pacman
				else if(!eaten && abs(ghostStatus.x - pacmanStats.x) < 2 && abs(ghostStatus.y - pacmanStats.y) < 2 && pacmanStats.status == 0){
					collisionStatus  = 1;
					xQueueSend(collisionQueue, &collisionStatus, 100);
				}
				// ghost is killed by pacmanghostRed, ghostBlue, ghostOrange, ghostPink
				else if(!eaten && abs(ghostStatus.x - pacmanStats.x) < 2 && abs(ghostStatus.y - pacmanStats.y) < 2 && pacmanStats.status == 1){
					//Draw an eye
					collisionStatus  = 2;
					eaten = 1;
					xQueueSend(collisionQueue, &collisionStatus, 100);
				}else if(eaten){
					eaten = 0;
				}
				if(MULTI_GHOST_STATE == FRIGHT){
						multiFrightTime+=1;
						if(multiFrightTime > 475)gdispImageDraw(&WHITE,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0);
						else if(multiFrightTime > 450) gdispImageDraw(&FRIGHTENED,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0);
						else if(multiFrightTime > 425) gdispImageDraw(&WHITE,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0);
						else if(multiFrightTime > 400) gdispImageDraw(&FRIGHTENED,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0);
						else if(multiFrightTime > 375) gdispImageDraw(&WHITE,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0);
						else if(multiFrightTime > 400) gdispImageDraw(&FRIGHTENED,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0);
						else if(multiFrightTime > 375) gdispImageDraw(&WHITE,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0);
						else if(multiFrightTime > 350) gdispImageDraw(&FRIGHTENED,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0);
						else if(multiFrightTime > 325) gdispImageDraw(&WHITE,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0);
						else if(multiFrightTime > 300) gdispImageDraw(&FRIGHTENED,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0);
						else if(multiFrightTime > 275) gdispImageDraw(&WHITE,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0);
						else gdispImageDraw(&FRIGHTENED,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0);

						if(multiFrightTime==500) multiFrightTime = 0;
				}else{
					switch(ghostStatus.direction){
						case GO_RIGHT:
							if(MULTI_GHOST_STATE == EATEN)gdispImageDraw(&EYE_RIGHT,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0);
							else{
								if(updatePlayer.player2 == ghostRed)gdispImageDraw(&RED_RIGHT,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0); // Red
								if(updatePlayer.player2 == ghostBlue)gdispImageDraw(&BLUE_RIGHT,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0); // Blue
								if(updatePlayer.player2 == ghostOrange)gdispImageDraw(&ORANGE_RIGHT,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0); // Orange
								if(updatePlayer.player2 == ghostPink)gdispImageDraw(&PINK_RIGHT,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0); // Pink
							}
							break;
						case GO_LEFT:
							if(MULTI_GHOST_STATE == EATEN)gdispImageDraw(&EYE_LEFT,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0);
							else{
								if(updatePlayer.player2 == ghostRed)gdispImageDraw(&RED_LEFT,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0);
								if(updatePlayer.player2 == ghostBlue)gdispImageDraw(&BLUE_LEFT,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0);
								if(updatePlayer.player2 == ghostOrange)gdispImageDraw(&ORANGE_LEFT,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0);
								if(updatePlayer.player2 == ghostPink)gdispImageDraw(&PINK_LEFT,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0);
							}
							break;
						case GO_UP:
							if(MULTI_GHOST_STATE == EATEN)gdispImageDraw(&EYE_UP,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0);
							else{
								if(updatePlayer.player2 == ghostRed)gdispImageDraw(&RED_UP,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0);
								if(updatePlayer.player2 == ghostBlue)gdispImageDraw(&BLUE_UP,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0);
								if(updatePlayer.player2 == ghostOrange)gdispImageDraw(&ORANGE_UP,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0);
								if(updatePlayer.player2 == ghostPink)gdispImageDraw(&PINK_UP,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0);
							}
							break;
						case GO_DOWN:
							if(MULTI_GHOST_STATE == EATEN)gdispImageDraw(&EYE_DOWN,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0);
							else{
								if(updatePlayer.player2 == ghostRed)gdispImageDraw(&RED_DOWN,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0);
								if(updatePlayer.player2 == ghostBlue)gdispImageDraw(&BLUE_DOWN,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0);
								if(updatePlayer.player2 == ghostOrange)gdispImageDraw(&ORANGE_DOWN,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0);
								if(updatePlayer.player2 == ghostPink)gdispImageDraw(&PINK_DOWN,ghostStatus.x-7,ghostStatus.y-7,14,14,0,0);
							}
							break;
						default:
							break;
					}
				}


			}else{
				unsigned char next_state_signal = statePauseMenuMulti;
				xQueueSend(stateChangeSignalQueue, &next_state_signal, 100);
				gdispDrawString(80, 120, str2, font1, White);
			}

		vTaskDelayUntil(&xLastWakeTime, tickFramerate);
		}
	}
}


