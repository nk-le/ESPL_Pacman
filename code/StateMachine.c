/*
 * StateMachine.c
 *
 *  Created on: Jun 28, 2019
 *      Author: lab_espl_stud11
 */

#include "StateMachine.h"
#include "gfx.h"
#include "Button.h"
#include "Demo.h"
#include "ghost.h"
#include "pac_man.h"

#define DISPLAY_SIZE_X  320
#define DISPLAY_SIZE_Y  240
#define DEFAULT 250

#define stateQueueLength 10

#define stateAmount 5

#define pacmanPlayer 1
#define ghostRed 2
#define ghostBlue 3
#define ghostOrange 4
#define ghostPink 5

#define PAUSED_TEXT_X(TEXT)     DISPLAY_SIZE_X / 2 - (gdispGetStringWidth(TEXT, font1) / 2)
#define PAUSED_TEXT_Y(LINE) DISPLAY_SIZE_Y / 2 - (gdispGetFontMetric(font1, fontHeight) * -(LINE + 0.5))

void drawTaskGameOver(void *param){
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	const TickType_t tickFramerate = 20;//refresh the screen every 20ms

	font_t font1; // Load font for ugfx
	font1 = gdispOpenFont("DejaVuSans24*");
	char str[2][70] = { "GAME OVER", "PRESS B TO VIEW THE SCORES"}; // buffer for messages to draw to display
	const unsigned char next_state_signal = stateHighScore;
	struct buttons buttonStatus = {{ 0 }}; // joystick queue input buffer

	while (1) {
		if (xSemaphoreTake(DrawReady, portMAX_DELAY) == pdTRUE) { // Block until screen is ready

			while (xQueueReceive(ButtonQueue, &buttonStatus, 0) == pdTRUE)
				;

			//SCREEN OUTPUT
				for (unsigned char i = 0; i < 2; i++)gdispDrawString(PAUSED_TEXT_X(str[i]), PAUSED_TEXT_Y(i), str[i], font1, White);
			// State machine Input
			if (buttonStatus.B)
				xQueueSend(stateChangeSignalQueue, &next_state_signal, 100);
		}
		vTaskDelayUntil(&xLastWakeTime, tickFramerate);
	}
}

//Menu to display when player wins the game
void drawTaskVictory(void *param){
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	const TickType_t tickFramerate = 20;//refresh the screen every 20ms
	font_t font1; // Load font for ugfx
	font1 = gdispOpenFont("DejaVuSans24*");
	char str[2][30] = { "Congratulations! You Win!", "PRESS B TO VIEW THE SCORES"}; // buffer for messages to draw to display
	const unsigned char next_state_signal = stateHighScore;
	struct buttons buttonStatus = {{ 0 }}; // joystick queue input buffer

	while (1) {
		if (xSemaphoreTake(DrawReady, portMAX_DELAY) == pdTRUE) { // Block until screen is ready

			while (xQueueReceive(ButtonQueue, &buttonStatus, 0) == pdTRUE)
				;

			//SCREEN OUTPUT
				for (unsigned char i = 0; i < 2; i++) gdispDrawString(PAUSED_TEXT_X(str[i]), PAUSED_TEXT_Y(i), str[i], font1, White);
			// State machine Input
			if (buttonStatus.B)
				xQueueSend(stateChangeSignalQueue, &next_state_signal, 100);
		}
		vTaskDelayUntil(&xLastWakeTime, tickFramerate);
	}
}

void drawTaskCheatMode(void *param){
		TickType_t xLastWakeTime;
		xLastWakeTime = xTaskGetTickCount();
		const TickType_t tickFramerate = 20;//refresh the screen every 20ms

		font_t font1; // Load font for ugfx
		font1 = gdispOpenFont("DejaVuSans24*");

		unsigned char strScore[10];
		char strLevel[3][5] = {"1", "2", "3"};
		char str[9][70] = { "WHO IS YOUR DADDY ?!", "Press B to quit", "Press C to start",
							"PAC-GOD", "PAC-GOLD", "PAC-LEVEL", "PAC-POWER", "ON", "OFF"}; // buffer for messages to draw to display
		unsigned char next_state_signal = statePlayingSingle;
		struct buttons buttonStatus = {{ 0 }}; // joystick queue input buffer
		unsigned char 	lastYDown = 0, lastYUp = 0, lastXDown = 0, lastXUp = 0, tmpState = 1, tmpLive = 0, tmpPower = 0;
		int tmpScore = 0;
		signed char tmpLevel = 0;
		struct cheat cheatUpdate;

		while (1) {
			if (xSemaphoreTake(DrawReady, portMAX_DELAY) == pdTRUE) { // Block until screen is ready

				while (xQueueReceive(ButtonQueue, &buttonStatus, 0) == pdTRUE)
					;

					//CHOOSING KINDS OF SETTING
					if(buttonStatus.joystick.y <= 25){
						if(lastYDown == 0){
							tmpState--;
							if(tmpState <= 1) tmpState = 1;
						}
						lastYDown--;
					}
					if(buttonStatus.joystick.y >= 200){
						if(lastYUp == 0){
							tmpState++;
							if(tmpState >= 4) tmpState = 4;
						}
						lastYUp++;
					}

					switch(tmpState){
						case 1: // Lives
							gdispFillCircle(90, 150, 4, Red);
							if(buttonStatus.joystick.x <= 25){
								tmpLive = 0;
							}
							if(buttonStatus.joystick.x >= 200){
								tmpLive = 1;
							}
							break;

						case 2: // Starting Score
							gdispFillCircle(90, 165, 4, Cyan);
							if(buttonStatus.joystick.x <= 25){
								tmpScore  -= 50;
								if(tmpScore <= 0){
									tmpScore = 0;
								}
							}
							if(buttonStatus.joystick.x >= 200){
								tmpScore  += 50;
							}
							break;

						case 3: // Starting Level
							gdispFillCircle(90, 180, 4, Orange);
							if(buttonStatus.joystick.x <= 25){
								if(lastXDown == 0){
									tmpLevel--;
									if(tmpLevel <= 0){
										tmpLevel = 0;
									}
								}
								lastXDown--;
							}
							if(buttonStatus.joystick.x >= 200){
								if(lastXUp == 0){
									tmpLevel++;
									if(tmpLevel >= 2){
										tmpLevel = 2;
									}
								}
								lastXUp++;
							}
							break;

						case 4: // InfinitPower
							gdispFillCircle(90, 195, 4, Pink);
							if(buttonStatus.joystick.x <= 25){
								tmpPower = 0;
							}
							if(buttonStatus.joystick.x >= 200){
								tmpPower = 1;
							}
							break;
					}

					// RESET JOYSTICK (DEBOUNCE)
					if(buttonStatus.joystick.y > 124 && buttonStatus.joystick.y < 132 )
					{
						lastYDown = 0;
						lastYUp = 0;
					}

					if(buttonStatus.joystick.x > 124 && buttonStatus.joystick.x < 132 )
					{
						lastXDown = 0;
						lastXUp = 0;
					}

					// UPDATE CHEAT
					cheatUpdate.level = tmpLevel;
					cheatUpdate.score = tmpScore;
					cheatUpdate.life  = tmpLive;
					cheatUpdate.power = tmpPower;

				// OUT PUT TO THE SCREEN
				// Lives Infinit
				if(!tmpLive) gdispDrawString(220, 145, str[8],font1, White); // Off
				else if(tmpLive) gdispDrawString(220, 145, str[7],font1, White); // On

				//Score
				sprintf(strScore, "%d", tmpScore);
				gdispDrawString(220, 160, strScore, font1, White);

				// Start Level, 1  2  3
				gdispDrawString(220, 175, strLevel[tmpLevel], font1, White);

				// Power Infinit
				if(!tmpPower) gdispDrawString(220, 190, str[8],font1, White); // Off
				else if(tmpPower) gdispDrawString(220, 190, str[7],font1, White); // On

				//SCREEN OUTPUT
				// GRAPHIC
				gdispFillArc(65,80,30,30,330,Red); //Pacman
				gdispFillCircle(105, 80, 4, Yellow);
				gdispFillCircle(125, 80, 4, Yellow);
				gdispFillCircle(145, 80, 4, Yellow);
				gdispImageDraw(&RED_RIGHT, 175, 70,14,14,0,0);
				gdispImageDraw(&BLUE_LEFT, 205, 70,14,14,0,0);
				gdispImageDraw(&ORANGE_UP, 235, 70,14,14,0,0);
				gdispImageDraw(&PINK_DOWN, 265, 70,14,14,0,0);

				//STRINGS
				gdispDrawString(100, 120, str[0],font1, White); // Cheat  Mode
				gdispDrawString(100, 145, str[3],font1, White); // Life
				gdispDrawString(100, 160, str[4],font1, White); // Score
				gdispDrawString(100, 175, str[5],font1, White); // Level
				gdispDrawString(100, 190, str[6],font1, White); // Power
				gdispDrawString(115, 215, str[1],font1, White); // Press B
				gdispDrawString(115, 225, str[2],font1, White); // Press C

				// GO TO NEXT STATE
				if (buttonStatus.B){
					next_state_signal = stateStartMenu;
					// RESET EVERYTHING BEFORE LEAVING
					tmpLevel = 0;
					tmpScore = 0;
					tmpLive = 0;
					tmpPower = 0;
					tmpState = 1;
					cheatUpdate.level = tmpLevel;
					cheatUpdate.score = tmpScore;
					cheatUpdate.life  = tmpLive;
					cheatUpdate.power = tmpPower;
					xQueueSend(stateChangeSignalQueue, &next_state_signal, 100);
				}
				if (buttonStatus.C){
					next_state_signal = statePlayingSingle;
					xQueueSend(cheatQueue, &cheatUpdate, 100); // Update Cheat
					// RESET EVERYTHING BEFORE LEAVING
					tmpLevel = 0;
					tmpScore = 0;
					tmpLive = 0;
					tmpPower = 0;
					tmpState = 1;
					cheatUpdate.level = tmpLevel;
					cheatUpdate.score = tmpScore;
					cheatUpdate.life  = tmpLive;
					cheatUpdate.power = tmpPower;
					xQueueSend(stateChangeSignalQueue, &next_state_signal, 100);
				}
			}
			vTaskDelayUntil(&xLastWakeTime, tickFramerate);
		}
}

void pacmanStateMachine(void *param) {
	volatile unsigned char stateChange = 1;
	unsigned char stateChangeSignal = stateStartMenu;//statePauseMenuMulti;//stateStartMenu;
	while(1)
	{
		// Handle state machine input
		if(stateChange)
					goto initial_state;

		//This function will read the queue which is written from another task to see if it attempts to change the state
		if (xQueueReceive(stateChangeSignalQueue, &stateChangeSignal, portMAX_DELAY) == pdTRUE) {
			stateChange = 1;
		}

		initial_state:
		if (stateChange)
		{
			switch (stateChangeSignal) {
			case stateStartMenu:
				vTaskResume(drawTaskStartMenuHandle);
				resetFoodMap();
				//SYSTEM
				vTaskSuspend(drawTaskPlayingSingleHandle);
				vTaskSuspend(drawTaskPauseMenuHandle);
				vTaskSuspend(drawTaskHighScoreHandle);
				vTaskSuspend(drawTaskGameOverHandle);
				vTaskSuspend(drawTaskVictoryHandle);
				vTaskSuspend(drawTaskCheatModeHandle);
				vTaskSuspend(drawTaskMultiMenuHandle);
				vTaskSuspend(drawTaskPlayingMultiHandle);
				vTaskSuspend(drawTaskPauseMultiMenuHandle);
				vTaskSuspend(uartUpdateGhostStatusHandle);
				vTaskSuspend(uartUpdatePacmanStatusHandle);
				//FIGURE
				vTaskSuspend(ghostRedHandle);
				vTaskSuspend(ghostBlueHandle);
				vTaskSuspend(ghostOrangeHandle);
				vTaskSuspend(ghostPinkHandle);
				vTaskSuspend(pacManHandle);
				vTaskSuspend(ghostMultiHandle);

				stateChange = 0;
				break;

			case statePauseMenu:
				vTaskResume(drawTaskPauseMenuHandle);

				vTaskSuspend(drawTaskStartMenuHandle);
				vTaskSuspend(drawTaskPlayingSingleHandle);
				vTaskSuspend(drawTaskHighScoreHandle);
				vTaskSuspend(drawTaskGameOverHandle);
				vTaskSuspend(drawTaskVictoryHandle);
				vTaskSuspend(drawTaskCheatModeHandle);
				vTaskSuspend(drawTaskMultiMenuHandle);
				vTaskSuspend(drawTaskPlayingMultiHandle);
				vTaskSuspend(drawTaskPlayingMultiHandle);
				//FIGURE
				vTaskSuspend(ghostRedHandle);
				vTaskSuspend(ghostBlueHandle);
				vTaskSuspend(ghostOrangeHandle);
				vTaskSuspend(ghostPinkHandle);
				vTaskSuspend(pacManHandle);
				vTaskSuspend(ghostMultiHandle);

				stateChange = 0;
				break;

			case stateHighScore:
				vTaskResume(drawTaskHighScoreHandle);

				vTaskSuspend(drawTaskStartMenuHandle);
				vTaskSuspend(drawTaskPlayingSingleHandle);
				vTaskSuspend(drawTaskPauseMenuHandle);
				vTaskSuspend(drawTaskGameOverHandle);
				vTaskSuspend(drawTaskVictoryHandle);
				vTaskSuspend(drawTaskCheatModeHandle);
				vTaskSuspend(drawTaskMultiMenuHandle);
				vTaskSuspend(drawTaskPlayingMultiHandle);
				vTaskSuspend(drawTaskPauseMultiMenuHandle);
				//FIGURE
				vTaskSuspend(ghostRedHandle);
				vTaskSuspend(ghostBlueHandle);
				vTaskSuspend(ghostOrangeHandle);
				vTaskSuspend(ghostPinkHandle);
				vTaskSuspend(pacManHandle);
				vTaskSuspend(ghostMultiHandle);

				stateChange = 0;
				break;

			case statePlayingSingle:
				vTaskResume(drawTaskPlayingSingleHandle);

				vTaskSuspend(drawTaskStartMenuHandle);
				vTaskSuspend(drawTaskPauseMenuHandle);
				vTaskSuspend(drawTaskHighScoreHandle);
				vTaskSuspend(drawTaskGameOverHandle);
				vTaskSuspend(drawTaskVictoryHandle);
				vTaskSuspend(drawTaskCheatModeHandle);
				vTaskSuspend(drawTaskMultiMenuHandle);
				vTaskSuspend(drawTaskPlayingMultiHandle);
				vTaskSuspend(drawTaskPauseMultiMenuHandle);

				stateChange = 0;
				break;

			case stateGameOver:
				vTaskResume(drawTaskGameOverHandle);

				// LEAVE FROM PLAYING SINGLE
				vTaskSuspend(drawTaskPlayingSingleHandle);

				// LEAVE FROM PLAYING MULTI
				vTaskSuspend(drawTaskPlayingMultiHandle);

				//FIGURE
				vTaskSuspend(ghostRedHandle);
				vTaskSuspend(ghostBlueHandle);
				vTaskSuspend(ghostOrangeHandle);
				vTaskSuspend(ghostPinkHandle);
				vTaskSuspend(pacManHandle);
				vTaskSuspend(ghostMultiHandle);
				vTaskSuspend(drawTaskPlayingMultiHandle);
				vTaskSuspend(drawTaskPauseMultiMenuHandle);

				stateChange = 0;
				break;

			case stateVictory:
				vTaskResume(drawTaskVictoryHandle);

				// LEAVE FROM PLAYING SINGLE
				vTaskSuspend(drawTaskPlayingSingleHandle);

				// LEAVE FROM PLAYING MULTI
				vTaskSuspend(drawTaskPlayingMultiHandle);

				//FIGURE
				vTaskSuspend(ghostRedHandle);
				vTaskSuspend(ghostBlueHandle);
				vTaskSuspend(ghostOrangeHandle);
				vTaskSuspend(ghostPinkHandle);
				vTaskSuspend(pacManHandle);
				vTaskSuspend(ghostMultiHandle);
				vTaskSuspend(drawTaskPlayingMultiHandle);
				vTaskSuspend(drawTaskPauseMultiMenuHandle);

				stateChange = 0;
				break;

			case stateCheatMode:
				vTaskResume(drawTaskCheatModeHandle);

				//LEAVE START MENU
				vTaskSuspend(drawTaskStartMenuHandle);

				stateChange = 0;
				break;

			case stateMultiMenu:
				vTaskResume(drawTaskMultiMenuHandle);
				vTaskResume(uartHandle); // Use for player Selection

				//LEAVE START MENU
				vTaskSuspend(drawTaskStartMenuHandle);

				stateChange = 0;
				break;

			case statePlayingMulti:
				vTaskResume(drawTaskPlayingMultiHandle);

				vTaskSuspend(uartHandle);

				//LEAVE START MENU
				vTaskSuspend(drawTaskMultiMenuHandle);

				//LEAVE PAUSE MENU MULTI
				vTaskSuspend(drawTaskPauseMultiMenuHandle);

				stateChange = 0;
				break;

			case statePauseMenuMulti:
				vTaskResume(drawTaskPauseMultiMenuHandle);
				vTaskResume(uartHandle); // Use for player Selection

				vTaskSuspend(uartUpdateGhostStatusHandle);
				vTaskSuspend(uartUpdatePacmanStatusHandle);
				vTaskSuspend(drawTaskPlayingMultiHandle);

//				vTaskSuspend(ghostRedHandle);
//				vTaskSuspend(ghostBlueHandle);
//				vTaskSuspend(ghostOrangeHandle);
//				vTaskSuspend(ghostPinkHandle);
				vTaskSuspend(pacManHandle);
				vTaskSuspend(ghostMultiHandle);

				stateChange = 0;
				break;

			default:
				stateChange = 0;
				break;
			}
		}
	}
}

void drawTaskPlayingSingle(void * params) {
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	const TickType_t tickFramerate = 20;//refresh the screen every 20ms

	struct buttons buttonStatus; // joystick queue input buffer
	unsigned char next_state_signal = statePauseMenuMulti, checkDie = 0;
	//volatile unsigned portBASE_TYPE uxHighWaterMarkSingle;
	//uxHighWaterMarkSingle = uxTaskGetStackHighWaterMark( NULL );

	// Start endless loop
	while (1) {
		if (xSemaphoreTake(DrawReady, portMAX_DELAY) == pdTRUE) { // Block until screen is ready
			while (xQueueReceive(ButtonQueue, &buttonStatus, 0) == pdTRUE)
			;

			unsigned char playMode = SINGLE_MODE;
			xQueueSend(playingModeQueue, &playMode, 0);

			drawMap(NULL);
			xQueueSend(buttonPacmanQueue,&buttonStatus,100); // Control Pacman

			vTaskResume(ghostRedHandle);
			vTaskResume(ghostBlueHandle);
			vTaskResume(ghostOrangeHandle);
			vTaskResume(ghostPinkHandle);
			vTaskResume(pacManHandle);

			xQueueReceive(pacmanDieQueue, &checkDie, 0); // Check Pacman Status

			// State machine input
			if(checkDie==1){ // If pacman dies
				next_state_signal = stateGameOver;
				checkDie = 0;
				xQueueSend(stateChangeSignalQueue, &next_state_signal, 100);
			}else if(checkDie==2){ // If pacman wins
				next_state_signal = stateVictory;
				checkDie = 0;
				xQueueSend(stateChangeSignalQueue, &next_state_signal, 100);
			}else if (buttonStatus.E){ // If pause
				next_state_signal = statePauseMenu;
				xQueueSend(stateChangeSignalQueue, &next_state_signal, 100);
			}
		}
		vTaskDelayUntil(&xLastWakeTime, tickFramerate);
	}
}

void drawTaskPauseMenu(void * params) {
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	const TickType_t tickFramerate = 20;//refresh the screen every 20ms

	font_t font1; // Load font for ugfx
	font1 = gdispOpenFont("DejaVuSans24*");
	char str[4][70] = { "PAUSE MENU", "CONTINUE", "QUIT TO MAIN MENU", "Press C to select"}; // buffer for messages to draw to display

	struct buttons buttonStatus = {{ 0 }}; // joystick queue input buffer
	unsigned char nextStateSelection = statePlayingSingle;
	unsigned char nextStateConfirmation = 0;
	unsigned char lastYDown = 0, lastYUp = 0, tmpState = 1;

	while (1) {
		if (xSemaphoreTake(DrawReady, portMAX_DELAY) == pdTRUE) { // Block until screen is ready

			while (xQueueReceive(ButtonQueue, &buttonStatus, 0) == pdTRUE)
				;
			//Choosing Next Mode
			if(buttonStatus.joystick.y <= 25){
				if(lastYDown == 0){
					tmpState--;
					if(tmpState <= 1) tmpState = 1;
				}
				lastYDown--;
			}
			if(buttonStatus.joystick.y >= 200){
				if(lastYUp == 0){
					tmpState++;
					if(tmpState >= 2) tmpState = 2;
				}
				lastYUp++;
			}

			if(buttonStatus.joystick.y > 124 && buttonStatus.joystick.y < 132 )
			{
				lastYDown = 0;
				lastYUp = 0;
			}

			if(tmpState == 1) // keep playing
			{
				nextStateSelection = statePlayingSingle;
				gdispFillCircle(100, 135, 4, Red);
			}
			else if(tmpState == 2) // Quit to main
			{
				nextStateSelection = stateStartMenu;
				gdispFillCircle(100, 150, 4, Cyan);
			}
			else;

			// OUTPUT TO SCREEN
			gdispDrawString(110, 100 , str[0],font1, White);
			gdispDrawString(110, 130 , str[1],font1, White);
			gdispDrawString(110, 145 , str[2],font1, White);
			gdispDrawString(110, 220 , str[3],font1, White);


			unsigned char checkC = buttonStatus.C;
			if(checkC) // Press C to choose
			{
				nextStateConfirmation = 1;
			}
			else
			{
				nextStateConfirmation = 0;
			}
			// State machine Input
			if (nextStateConfirmation){
				nextStateConfirmation = 0;
				xQueueSend(stateChangeSignalQueue, &nextStateSelection, 100);
				checkC = 0;
				tmpState = 1;
			}
		}
		vTaskDelayUntil(&xLastWakeTime, tickFramerate);
	}
}

void drawTaskStartMenu(void * params) {
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	const TickType_t tickFramerate = 20;//refresh the screen every 20ms

	font_t font1; // Load font for ugfx
	font1 = gdispOpenFont("DejaVuSans24*");
	char str[8][70] = { "START MENU", "SINGLE PLAY", "MULTIPLAYER", "CHEAT MODE",  "HIGH SCORE", "Press C to choose", "PACMAN - COVERED BY","KHANH & PETER"}; // buffer for messages to draw to display
	struct buttons buttonStatus = {{ 0 }}; // joystick queue input buffer

	unsigned char nextStateSelection = statePlayingSingle; // Setup nexet State
	unsigned char tmpState = 1;
	unsigned char nextStateConfirmation = 0;

	unsigned char restart = 1;
	char lastYDown = 0, lastYUp = 0;

	while (1) {
		if (xSemaphoreTake(DrawReady, portMAX_DELAY) == pdTRUE) { // Block until screen is ready
			while (xQueueReceive(ButtonQueue, &buttonStatus, 0) == pdTRUE)
				;
			xQueueSend(restartQueue, &restart, 0);
			unsigned char checkC = buttonStatus.C;

			//CHOOSING NEXT STATE
			if(buttonStatus.joystick.y <= 25){
				if(lastYDown == 0){
					tmpState--;
					if(tmpState <= 1) tmpState = 1;
				}
				lastYDown--;
			}
			if(buttonStatus.joystick.y >= 200){
				if(lastYUp == 0){
					tmpState++;
					if(tmpState >= 4) tmpState = 4;
				}
				lastYUp++;
			}

			if(buttonStatus.joystick.y > 124 && buttonStatus.joystick.y < 132 )
			{
				lastYDown = 0;
				lastYUp = 0;
			}

			if(tmpState == 1)
			{
				nextStateSelection = statePlayingSingle;
				gdispFillCircle(120, 145 , 4, Red); // Start
			}
			else if(tmpState == 2)
			{
				nextStateSelection = stateMultiMenu;
				gdispFillCircle(120, 160, 4, Cyan);
			}
			else if(tmpState == 3)
			{
				nextStateSelection = stateCheatMode;
				gdispFillCircle(120, 175, 4, Orange);
			}
			else if(tmpState == 4)
			{
				nextStateSelection = stateHighScore;
				gdispFillCircle(120, 190, 4, Pink);
			}
			else;

			//SCREEN OUTPUT
			//int pacmanStartX = 80, pacmanStartY = 80, pacmanSize = 40,
			gdispFillArc(65,80,30,30,330,Yellow); //Pacman
			gdispFillCircle(105, 80, 4, Yellow);
			gdispFillCircle(125, 80, 4, Yellow);
			gdispFillCircle(145, 80, 4, Yellow);
			gdispImageDraw(&RED_RIGHT, 175, 70,14,14,0,0);
			gdispImageDraw(&BLUE_LEFT, 205, 70,14,14,0,0);
			gdispImageDraw(&ORANGE_UP, 235, 70,14,14,0,0);
			gdispImageDraw(&PINK_DOWN, 265, 70,14,14,0,0);

			gdispDrawString(130, 120, str[0],font1, White); // Start Menu
			gdispDrawString(130, 140, str[1],font1, White); // Single
			gdispDrawString(130, 155, str[2],font1, White); // Multi
			gdispDrawString(130, 170, str[3],font1, White); // Cheat
			gdispDrawString(130, 185, str[4],font1, White); // High Score
			gdispDrawString(130, 220, str[5],font1, White); // Press C
			gdispDrawString(165, 35, str[6],font1, White); // PACMAN
			gdispDrawString(185, 50, str[7],font1, White); // KHANH & PETER

			if(checkC) // Press C to choose
			{
				nextStateConfirmation = 1;
			}
			else
			{
				nextStateConfirmation = 0;
			}
			// State machine Input
			if (nextStateConfirmation){
				nextStateConfirmation = 0;
				unsigned char playMode = SINGLE_MODE;
				xQueueSend(playingModeQueue, &playMode, 100);
				xQueueSend(stateChangeSignalQueue, &nextStateSelection, 100);
				checkC = 0;
				tmpState = 1 ;
			}
		}
		vTaskDelayUntil(&xLastWakeTime, tickFramerate);
	}
}

void drawTaskHighScore(void * params) {
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	const TickType_t tickFramerate = 20;//refresh the screen every 20ms
	int score = 0;
	int scoreMulti = 0;
	int highScore[4] = {0};
	int highScoreMulti[4] = {0};

	font_t font1; // Load font for ugfx
	font1 = gdispOpenFont("DejaVuSans24*");
	char str[4][70] = { "HIGH SCORE", "Press B to get Back", "SINGLE PLAYER", "MULTIPLAYER" }; // buffer for messages to draw to display
	char str1[30];

	//struct buttons buttonStatus; // joystick queue input buffer
	const unsigned char next_state_signal = stateStartMenu;
	struct buttons buttonStatus = {{ 0 }}; // joystick queue input buffer

	while (1) {
		if (xSemaphoreTake(DrawReady, portMAX_DELAY) == pdTRUE) { // Block until screen is ready

			while (xQueueReceive(ButtonQueue, &buttonStatus, 0) == pdTRUE)
				;
			xQueueReceive(scoreQueue, &score, 0); //TAKE THE SCORE IF PACMAN DIES
			xQueueReceive(scoreMultiQueue, &scoreMulti, 0); //TAKE THE SCORE IF PACMAN DIES

			unsigned char i,j  = 0;

			if(score > highScore[0])
			highScore[0] = score; // save the score into the last position and sort
			highScoreMulti[0] = scoreMulti;

			//Sorting
			//SINGLE
			for (i = 0; i < 4; i++){
				for(j = i +1; j < 4; j++){
					if(highScore[i] > highScore[j])
					{
						int tmpSwap = highScore[i];
						highScore[i] = highScore[j];
						highScore[j] = tmpSwap;
					}
				}
			}

			//MULTI
			for (i = 0; i < 4; i++){
				for(j = i +1; j < 4; j++){
					if(highScoreMulti[i] > highScoreMulti[j])
					{
						int tmpSwap = highScoreMulti[i];
						highScoreMulti[i] = highScoreMulti[j];
						highScoreMulti[j] = tmpSwap;
					}
				}
			}
			//SETBACK SCORE VARIABLE
			score = 0;
			scoreMulti = 0;

			//OUTPUT SCREEN
			//SINGLE PLAYER
			sprintf(str1, "1st	 %d", highScore[3]);
			gdispDrawString(40, 80, str1,font1, White);
			sprintf(str1, "2nd	 %d", highScore[2]);
			gdispDrawString(40, 90, str1,font1, White);
			sprintf(str1, "3rd	 %d", highScore[1]);
			gdispDrawString(40, 100, str1,font1, White);
			sprintf(str1, "4th	 %d", highScore[0]);
			gdispDrawString(40, 110, str1,font1, White);

			//SINGLE PLAYER
			sprintf(str1, "1st	 %d", highScoreMulti[3]);
			gdispDrawString(40, 160, str1,font1, White);
			sprintf(str1, "2nd	 %d", highScoreMulti[2]);
			gdispDrawString(40, 170, str1,font1, White);
			sprintf(str1, "3rd	 %d", highScoreMulti[2]);
			gdispDrawString(40, 180, str1,font1, White);
			sprintf(str1, "4th	 %d", highScoreMulti[2]);
			gdispDrawString(40, 190, str1,font1, White);

			//SCREEN OUTPUT
			gdispDrawString(130, 30, str[0],font1, White); // HIGH SCORE MENU

			gdispDrawString(30, 60, str[2],font1, White); // Single
			gdispFillArc(220,90,15,30,330,Yellow); //Pacman Next To
			gdispFillCircle(250, 90, 4, Yellow);
			gdispFillCircle(265, 90, 4, Yellow);
			gdispFillCircle(280, 90, 4, Yellow);

			gdispDrawString(30, 140, str[3],font1, White); // Multi
			gdispImageDraw(&RED_RIGHT, 235, 150,14,14,0,0);
			gdispImageDraw(&BLUE_LEFT, 265, 150,14,14,0,0);
			gdispImageDraw(&ORANGE_UP, 235, 170,14,14,0,0);
			gdispImageDraw(&PINK_DOWN, 265, 170,14,14,0,0);

			gdispDrawString(120, 220, str[1],font1, White); // PRESS B

			// State machine Input
			if (buttonStatus.B)
				xQueueSend(stateChangeSignalQueue, &next_state_signal, 100);
		}
		vTaskDelayUntil(&xLastWakeTime, tickFramerate);

	}
}

void drawTaskMultiMenu(void *param){
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	const TickType_t tickFramerate = 20;//refresh the screen every 20ms

	font_t font1; // Load font for ugfx
	font1 = gdispOpenFont("DejaVuSans24*");

	struct buttons buttonStatus = {{ 0 }}; // joystick queue input buffer
	struct buttons uartSelection = {{ 0 }}; // joystick queue input buffer
	struct buttons selectionSend = {{ 0 }}; // joystick queue input buffer
	struct multiplayer updatePlayer = {0,0}; // joystick queue input buffer

	struct ghost ghost ;
	unsigned char selection = pacmanPlayer, selectionUart;
	unsigned char player1Ready = 0;
	unsigned char player2Ready = 0;

	char str[9][70] = { "MULTIPLAYER MODE", "Press B to leave (deselect first)", "Press C to select / deselect","PACMAN", "GHOST", "READY", "NOT AVAILABLE", "OPPONENT","NOT READY"}; // buffer for messages to draw to display

	unsigned char next_state_signal = statePlayingSingle;

	unsigned char lastYDown = 0, lastYUp = 0, lastX = 0,tmpState = 1, tmpGhost = ghostRed;

	while(1){
		if (xSemaphoreTake(DrawReady, portMAX_DELAY) == pdTRUE) { // Block until screen is ready
			while (xQueueReceive(ButtonQueue, &buttonStatus, 0) == pdTRUE)
							;
			xQueueReceive(uartButtonQueue, &uartSelection, 0);

			// ONLY ABLE TO SETUP WHEN PLAYER IS NOT READY
			if(!player1Ready){
				if(buttonStatus.joystick.y <= 25){
					if(lastYDown == 0){
						tmpState--;
						if(tmpState <= 1) tmpState = 1;
					}
					lastYDown--;
				}
				if(buttonStatus.joystick.y >= 200){
					if(lastYUp == 0){
						tmpState++;
						if(tmpState >= 2) tmpState = 2;
					}
					lastYUp++;
				}

				switch(tmpState){
					case 1: // PACMAN
						gdispFillCircle(100, 120, 4, Red);
						break;

					case 2: // GHOST
						gdispFillCircle(100, 140, 4, Cyan);
						if(buttonStatus.joystick.x <= 25){
							if(lastX == 0){
								tmpGhost--;
								if(tmpGhost <= 2) tmpGhost = 2;
							}
							lastX--;
							}
						if(buttonStatus.joystick.x >= 200){
							if(lastX == 0){
								tmpGhost++;
								if(tmpGhost >= 5) tmpGhost = 5;
							}
							lastX--;
						}
						break;
				}
				if(buttonStatus.joystick.y > 124 && buttonStatus.joystick.y < 132 )
				{
					lastYDown = 0;
					lastYUp = 0;
				}

				if(buttonStatus.joystick.x > 124 && buttonStatus.joystick.x < 132 )
				{
					lastX = 0;
				}
			}

			// OUT PUT MENU SCREEN
			//CHECK MY PLAYER
			if (tmpState == 1){
				gdispFillArc(255,120,6,50,310,Yellow);
				selection = pacmanPlayer;
			}
			else{
				switch(tmpGhost){
					case ghostRed:
						ghost.x = RED_INIT_X;
						ghost.y = RED_INIT_Y;
						ghost.speed = 1;
						gdispImageDraw(&RED_RIGHT,250,130,14,14,0,0);
						selection = ghostRed;
						break;
					case ghostBlue:
						ghost.x = BLUE_INIT_X;
						ghost.y = BLUE_INIT_Y;
						ghost.speed = 1;
						gdispImageDraw(&BLUE_RIGHT,250,130,14,14,0,0);
						selection = ghostBlue;
						break;
					case ghostOrange:
						ghost.x = ORANGE_INIT_X;
						ghost.y = ORANGE_INIT_Y;
						ghost.speed = 1;
						gdispImageDraw(&ORANGE_RIGHT,250,130,14,14,0,0);
						selection = ghostOrange;
						break;
					case ghostPink:
						ghost.x = PINK_INIT_X;
						ghost.y = PINK_INIT_Y;
						ghost.speed = 1;
						gdispImageDraw(&PINK_RIGHT,250,130,14,14,0,0);
						selection = ghostPink;
						break;
					}
			}
			// CHECK ANOTHER PLAYER AND OUTPUT SCREEN THE SELECTION OF ANOTHER PLAYER
			switch(uartSelection.A){
				case pacmanPlayer:
					gdispFillArc(255,155,6,50,310,Yellow);
					selectionUart = pacmanPlayer;
					break;
				case ghostRed:
					ghost.x = RED_INIT_X;
					ghost.y = RED_INIT_Y;
					ghost.speed = 1;
					gdispImageDraw(&RED_RIGHT,250,155,14,14,0,0);
					selectionUart = ghostRed;
					break;
				case ghostBlue:
					ghost.x = BLUE_INIT_X;
					ghost.y = BLUE_INIT_Y;
					ghost.speed = 1;
					gdispImageDraw(&BLUE_RIGHT,250,155,14,14,0,0);
					selectionUart = ghostBlue;
					break;
				case ghostOrange:
					ghost.x = ORANGE_INIT_X;
					ghost.y = ORANGE_INIT_Y;
					ghost.speed = 1;
					gdispImageDraw(&ORANGE_RIGHT,250,155,14,14,0,0);
					selectionUart = ghostOrange;
					break;
				case ghostPink:
					ghost.x = PINK_INIT_X;
					ghost.y = PINK_INIT_Y;
					ghost.speed = 1;
					gdispImageDraw(&PINK_RIGHT,250,155,14,14,0,0);
					selectionUart = ghostPink;
					break;
				}


			//CONFIG PLAYER 1 SELECTION
			if(buttonStatus.C) player1Ready = !player1Ready;

			//SEND ACTUAL SELECTION TO 2ND PLAYER
			selectionSend.A = selection;
			selectionSend.C = player1Ready;
			sendPosition(selectionSend); // SEND TO UART BOARD

			// CHECK PLAYER 2 SELECTION
			player2Ready = uartSelection.C; // Check if player 2 Ready

			//OUTPUT TO SCREEN
			gdispDrawString(110, 80 , str[0], font1, White); // Menu
			gdispDrawString(110, 115 , str[3], font1, White); // Pacman
			gdispDrawString(110, 135 , str[4], font1, White); // Ghost
			gdispDrawString(110, 155 , str[7], font1, White); // Opponent

			gdispDrawString(110, 200 , str[1], font1, White); // Press to leave
			gdispDrawString(110, 220 , str[2], font1, White); // Press to start

			if(player1Ready){
				if(selection == pacmanPlayer){
					gdispDrawString(20, 115, str[5], font1, White); // Ready
				}else if (selection != pacmanPlayer){
					gdispDrawString(20,135, str[5], font1, White); // Ready
				}
			}else{
				if(selection == pacmanPlayer){
					gdispDrawString(20, 115, str[8], font1, White); // Ready
				}else if (selection != pacmanPlayer){
					gdispDrawString(20, 135, str[8], font1, White); // Ready
				}
			}

			if(player2Ready) {
				// IF ANOTHER PLAYER ALREADY MAKE DECISION SO WE CAN NOT HAVE THE SAME FIGURE
				if(selectionUart == pacmanPlayer){
					tmpState = 2;
					gdispDrawString(220, 115, str[6], font1, White); //
					gdispDrawString(20, 155, str[5], font1, White); //
				}else if (selectionUart != pacmanPlayer){
					tmpState = 1;
					gdispDrawString(220, 135, str[6], font1, White); //
					gdispDrawString(20, 155, str[5], font1, White); //
				}
			}else{
					gdispDrawString(20, 155, str[8], font1, White); //
			}

			// CHANGE STATE
			// QUIT TO MAIN MENU
			if(!player1Ready){
				if (buttonStatus.B){
					next_state_signal = stateStartMenu;
					xQueueSend(stateChangeSignalQueue, &next_state_signal, 100);
					tmpState = 1;
					tmpGhost = ghostRed;
					player1Ready = 0;
				}
			}
			// START PLAYING MULTIPLAYER
			if (player1Ready && player2Ready){
				vTaskSuspend(uartHandle);

				//UPDATE GHOST MULTIPLAYER
				xQueueSend(ghostMultiQueue, &ghost, 100);

				//UPDATE PLAYER SELECTION
				updatePlayer.player1 = selection;
				updatePlayer.player2 = selectionUart;
				xQueueSend(playerSelectQueue, &updatePlayer, 100);
				xQueueSend(ghostMultiReceiveQueue, &updatePlayer, 100);

				unsigned char playMode = MULTI_MODE;
				xQueueSend(playingModeQueue, &playMode, 0);

				unsigned char synchro = 1; //STARTING SIGNAL
				xQueueSend(synchroQueue, &synchro, 0);

				// SUSPEND AND ACTIVE TASKS BEFORE LEAVING
				if(updatePlayer.player1 == pacmanPlayer){ // OWN BOARD
					vTaskResume(pacManHandle);
					vTaskResume(uartUpdateGhostStatusHandle);
					vTaskResume(ghostMultiReceiveHandle);
				}else{ // UART BOARD
					vTaskResume(uartUpdatePacmanStatusHandle);
					vTaskResume(ghostMultiHandle);
					gdispDrawString(PAUSED_TEXT_X(str[0]), PAUSED_TEXT_Y(0), str[0], font1, White);
				}


				unsigned char tmpSelection;
				if(selection == pacmanPlayer) tmpSelection = selectionUart;
				else tmpSelection = selection;

				switch(tmpSelection){
					case ghostRed:
						vTaskResume(ghostBlueHandle);
						vTaskResume(ghostOrangeHandle);
						vTaskResume(ghostPinkHandle);
						vTaskSuspend(ghostRedHandle);

						break;
					case ghostBlue:
						vTaskResume(ghostRedHandle);
						vTaskResume(ghostOrangeHandle);
						vTaskResume(ghostPinkHandle);
						vTaskSuspend(ghostBlueHandle);

						break;
					case ghostOrange:
						vTaskResume(ghostBlueHandle);
						vTaskResume(ghostRedHandle);
						vTaskResume(ghostPinkHandle);
						vTaskSuspend(ghostOrangeHandle);

						break;
					case ghostPink:
						vTaskResume(ghostBlueHandle);
						vTaskResume(ghostOrangeHandle);
						vTaskResume(ghostRedHandle);
						vTaskSuspend(ghostPinkHandle);
						break;
					}

				//RESET EVERYTHING BEFORE LEAVING
				tmpState = 1;
				tmpGhost = ghostRed;
				player1Ready = 0;
				player2Ready = 0;
				selectionSend.C = 0;
				selectionSend.A = 0;
				uartSelection.A = 0;
				uartSelection.C = 0;
				updatePlayer.player1 = DEFAULT;
				updatePlayer.player2 = DEFAULT;
				selection = DEFAULT;
				selectionUart = DEFAULT;

				//GO TO NEXT STATE
				next_state_signal = statePlayingMulti;
				xQueueSend(stateChangeSignalQueue, &next_state_signal, 100);
			}
			vTaskDelayUntil(&xLastWakeTime, tickFramerate);
		}
	}
}

void drawTaskPlayingMulti(void *param){
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	const TickType_t tickFramerate = 20;//refresh the screen every 20ms

	font_t font1; // Load font for ugfx
	font1 = gdispOpenFont("DejaVuSans24*");
	char str[2][70] = { "PLAYER GHOST", "PACMAN"}; // buffer for messages to draw to display
	unsigned char next_state_signal = stateHighScore;
	struct buttons buttonStatus = {{ 0 }}; // joystick queue input buffer
	struct multiplayer updatePlayer;
	struct pacman pacman;
	struct ghost ghost;
	unsigned char playMode = MULTI_MODE;

	unsigned char synchro = 0;
	unsigned char checkDie = 0;
	unsigned char checkUartCable = 1;

	while (1) {
		if (xSemaphoreTake(DrawReady, portMAX_DELAY) == pdTRUE) { // Block until screen is ready

			xQueueSend(playingModeQueue, &playMode, 0);
			xQueueReceive(checkUartQueue, &checkUartCable, 0);
			while (xQueueReceive(ButtonQueue, &buttonStatus, 0) == pdTRUE)
			;
			// CLEAR EVERTHING INSIDE UART BUFFER
			if(xQueueReceive(synchroQueue, &synchro, 0) == pdTRUE){
				xQueueReceive(playerSelectQueue, &updatePlayer, 0);
			}

			drawMap(NULL);
			if(updatePlayer.player1 == pacmanPlayer){ // Own Board is pacman
				vTaskResume(pacManHandle); // Control Pacman
				vTaskResume(uartUpdateGhostStatusHandle); // Receive Ghost
				vTaskResume(ghostMultiReceiveHandle); // Ghost is controlled through UART
				gdispDrawString(230, 130, str[1], font1, White);
				xQueueSend(buttonPacmanQueue, &buttonStatus, 0); // Control Pacman through Own Board

			}else{ //Own board is ghost
				vTaskResume(uartUpdatePacmanStatusHandle); // Receive Pacman
				vTaskResume(ghostMultiHandle); // Control Ghost , draw Pacman and send Signal through Uart
				gdispDrawString(230,130, str[0], font1, White);
				xQueueSend(buttonGhostQueue, &buttonStatus, 0); // Control Ghost trough own board
			}

			xQueueReceive(pacmanDieQueue, &checkDie, 0); // Check Pacman Status

			// State machine input
			if(checkDie){
				checkDie = 0;
				next_state_signal = stateGameOver;
				xQueueSend(stateChangeSignalQueue, &next_state_signal, 100);
			}
			else if (buttonStatus.E || !checkUartCable){
				if(updatePlayer.player1 == pacmanPlayer){ // Own Board
					vTaskSuspend(uartUpdateGhostStatusHandle);
					pacman.uartCheck = PAUSING;
					sendStatusPacman(pacman); // Only for 2 player Mode
				}else{ // Another Board
					vTaskSuspend(uartUpdatePacmanStatusHandle);
					ghost.uartCheck = PAUSING;
					sendStatusGhost(ghost); // Only for 2 player Mode
					vTaskSuspend(ghostMultiHandle); // Control Pacman
				}
				next_state_signal = statePauseMenuMulti;
				vTaskSuspend(pacManHandle); // Control Pacman
				xQueueSend(stateChangeSignalQueue, &next_state_signal, 100);
			}
			checkUartCable = 0; // Reset to check the UART Cable again.
		}
		vTaskDelayUntil(&xLastWakeTime, tickFramerate);
	}
}

void drawTaskPauseMultiMenu(){
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	const TickType_t tickFramerate = 20;//refresh the screen every 20ms

	font_t font1; // Load font for ugfx
	font1 = gdispOpenFont("DejaVuSans24*");

	struct buttons buttonStatus = {{ 0 }};
	struct buttons uartSelection = {{ 0 }};
	struct buttons selectionSend = {{ 0 }};
	struct multiplayer updatePlayer;

	unsigned char player1Ready = 0;
	unsigned char player2Ready = 0;
	const unsigned char playerLeft = 5;

	char str[7][70] = { "MULTIPLAYER MODE", "PAUSE MENU", "PRESS C KEEP PLAYING","QUIT", "WAITING FOR OPPONENT", "OPPONENT IS WAITING", "OPPONENT LEFT"}; // buffer for messages to draw to display

	unsigned char next_state_signal = statePlayingSingle;
	unsigned char lastYDown = 0, lastYUp = 0,tmpState = 1;


	while(1){
		if (xSemaphoreTake(DrawReady, portMAX_DELAY) == pdTRUE) { // Block until screen is ready
			while (xQueueReceive(ButtonQueue, &buttonStatus, 0) == pdTRUE)
							;
			xQueueReceive(uartButtonQueue, &uartSelection, 0);

			// ONLY ABLE TO SETUP WHEN NOT READY
			if(!player1Ready){
				for (unsigned char i = 0; i < 4; i++){
					gdispDrawString(PAUSED_TEXT_X(str[i]), PAUSED_TEXT_Y(i), str[i], font1, White);
				}

				//Choosing Next Mode
				if(buttonStatus.joystick.y <= 25){
					if(lastYDown == 0){
						tmpState--;
						if(tmpState <= 1) tmpState = 1;
					}
					lastYDown--;
				}
				if(buttonStatus.joystick.y >= 200){
					if(lastYUp == 0){
						tmpState++;
						if(tmpState >= 2) tmpState = 2;
					}
					lastYUp++;
				}

				switch(tmpState){
					case 1: // KEEP PLAYING
						gdispFillCircle(20, PAUSED_TEXT_Y(2) + 5, 6, Red);
						break;

					case 2: // QUIT TO MAIN MENU
						gdispFillCircle(20, PAUSED_TEXT_Y(3) + 5, 6, Blue);
						break;
					default: break;
				}

				if(buttonStatus.joystick.y > 124 && buttonStatus.joystick.y < 132 )
				{
					lastYDown = 0;
					lastYUp = 0;
				}
			}

			//CONFIG OWN BOARD
			if(buttonStatus.C){
				if(tmpState == 1) player1Ready = !player1Ready; // CONTINUE
				else{ //QUIT TO MAIN
					// UPDATE UART
					selectionSend.K = playerLeft;
					sendPosition(selectionSend);

					//RESET EVERYTHING TO LEAVE
					vTaskSuspend(uartHandle);
					tmpState = 1;
					uartSelection.E = 0;
					uartSelection.K = 0;
					player1Ready = 0;
					player2Ready = 0;
					selectionSend.K = 0;
					selectionSend.E = 0;
					buttonStatus.C = 0;
					next_state_signal = stateStartMenu;
					xQueueSend(stateChangeSignalQueue, &next_state_signal, 100);
				}
			}

			//SEND ACTUAL SELECTION TO 2ND PLAYER
			selectionSend.E = player1Ready;  // Send Ready Signal
			sendPosition(selectionSend); // SEND TO UART BOARD

			// CHECK PLAYER 2 SELECTION
			player2Ready = uartSelection.E; // Check if P2 Ready
			if(uartSelection.K == playerLeft){
				next_state_signal = stateStartMenu;
				gdispDrawString(PAUSED_TEXT_X(str[6]), PAUSED_TEXT_Y(6) + 25, str[6], font1, White);
			}

			// OUT PUT MENU SCREEN
			if(player1Ready) // WAITING FOR OPPONENT
				gdispDrawString(PAUSED_TEXT_X(str[4]), PAUSED_TEXT_Y(4) + 25, str[4], font1, White);

			if(player2Ready) // IF ANOTHER PLAYER ALREADY, PRINT WAITING
				gdispDrawString(PAUSED_TEXT_X(str[5]), PAUSED_TEXT_Y(5) + 25, str[5], font1, White);

			// 2 Players are ready, reset variables
			if (player1Ready && player2Ready){
				//SETUP EVERYTHING BACK BEFORE LEAVING
				tmpState = 1;
				uartSelection.E = 0;
				uartSelection.K = 0;
				player1Ready = 0;
				player2Ready = 0;
				selectionSend.K = 0;
				selectionSend.E = 0;
				vTaskSuspend(uartHandle);

				// GO TO NEXT STATE
				next_state_signal = statePlayingMulti;
				xQueueSend(stateChangeSignalQueue, &next_state_signal, 100); // check here to keep playings
			}
		}
		vTaskDelayUntil(&xLastWakeTime, tickFramerate);
	}
}








