/*
 * StateMachine.h
 *
 *  Created on: Jun 28, 2019
 *      Author: lab_espl_stud11
 */

#ifndef CODE_STATEMACHINE_H_
#define CODE_STATEMACHINE_H_
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// Define to check UART
#define PAUSING 4
#define PLAYING 5

// Define States of State Machine
#define stateUndefined 0
#define stateStartMenu 1
#define statePauseMenu 2
#define statePlayingSingle 3
#define stateHighScore 4
#define stateMultiMenu 5
#define stateCheatMode 6
#define stateGameOver 7
#define statePlayingMulti 8
#define statePauseMenuMulti 9
#define stateVictory 10

//Define Ghost Colors
#define colorRed 0
#define colorOrange 1
#define colorBlue 2
#define colorPink 3

//Task Communication Queues
//extern QueueHandle_t stateStatusQueue;
extern QueueHandle_t stateChangeSignalQueue; //Bit: which announce that the state was changed
extern QueueHandle_t buttonGhostQueue; // Button to control the ghost
extern QueueHandle_t synchroQueue; // synchronization Signal for multiplayer Mode

//Task Handles of State Machine
extern TaskHandle_t frameSwapHandle;
extern TaskHandle_t drawTaskStartMenuHandle;
extern TaskHandle_t drawTaskPauseMenuHandle;
extern TaskHandle_t drawTaskPlayingSingleHandle;
extern TaskHandle_t drawTaskPlayingMultiHandle;
extern TaskHandle_t drawTaskMultiMenuHandle;
extern TaskHandle_t drawTaskHighScoreHandle;
extern TaskHandle_t drawTaskGameOverHandle;
extern TaskHandle_t drawTaskVictoryHandle;
extern TaskHandle_t drawTaskCheatModeHandle;
extern TaskHandle_t stateMachineHandle;
extern TaskHandle_t drawTaskPauseMultiMenuHandle;

//Task Handles for UART Communication
extern TaskHandle_t uartHandle;
extern TaskHandle_t uartUpdatePacmanStatusHandle;
extern TaskHandle_t uartUpdateGhostStatusHandle;
extern TaskHandle_t ghostMultiReceiveHandle;

//Task Handles of Objects
extern TaskHandle_t ghostRedHandle;
extern TaskHandle_t ghostBlueHandle;
extern TaskHandle_t ghostPinkHandle;
extern TaskHandle_t ghostOrangeHandle;
extern TaskHandle_t pacManHandle;
extern TaskHandle_t ghostMultiHandle;
extern TaskHandle_t pacmanMultiHandle;
extern TaskHandle_t ghostNewMultiHandle;


// STATE MACHINE OF THE WHOLE GAME, WHICH WILL MANAGE ANOTHER TASKS
void pacmanStateMachine(void *param);

/*
Main Menu Task - State 0
This state will run at the beginning of the game, in which player can choose playing Mode.
Mode Seletion will be performed through joystick and choose button. For example go up/down and select/go back:
																								Playing Single
		Pause Menu				=================>		Start Menu		=================>		Playing Multi
		High Score																				Viewing High Score
*/
void drawTaskStartMenu(void *param);


/*
Pause Menu Task - State
This state will run during the game, player can pause the game with it.
Mode Seletion will be performed through joystick and choose button. For example go up/down and select/go back:
		Playing Single																			Continue Playing Single
		Playing Multi			=================>		Pause Menu		=================>
		High Score (Get Back)																	Quit to start menu
*/
void drawTaskPauseMenu(void *param);


/*
Playing Single Task - State
This state will run the game in single Mode.
		Start Menu				=================>	  Playing Single	=================>		Pause Menu
*/
void drawTaskPlayingSingle(void *param);


/*
Playing GameOver - State
This state will run when pacman has no more lives
		Playing Single				=================>	  Game Over 	=================>		High Score
*/
void drawTaskGameOver(void *param);

/*
Playing Victory - State
This state will run when the player wins the game
		Playing Single				=================>	  Game Over 	=================>		High Score
*/
void drawTaskVictory(void *param);

/*
High Score Task - State
This state will show the High Score
		Pause Menu																				Pause Menu
		Start Menu				=================>		High Score		=================>		Start Menu
*/
void drawTaskHighScore(void *param);


/*
Cheat Mode - State
This state will show the High Score
		Start Menu				=================>		Cheat Mode		=================>		Playing Single
*/
void drawTaskCheatMode(void *param);



/*
Multiplayer Menu - State
This state will show the High Score
		Start Menu				=================>		Multiplayer Menu	=================>		Playing Multi
*/
void drawTaskMultiMenu(void *param);

/*
Playing Multiplayer - State
This state will show the High Score
		Multiplayer Menu		=================>		Playing Multi		=================>		Pause Menu
*/
void drawTaskPlayingMulti(void *param);

/*
Pause Menu Multi - State
This state will pause the game of multiplayer mode
		Playing Multiplayer  	=================>		Pause Menu Multi	=================>		Quit to start menu
																									Playing Multiplayer
*/
void drawTaskPauseMultiMenu();

#endif /* CODE_STATEMACHINE_H_ */
