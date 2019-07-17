/*
 * ghost.h
 *
 *  Created on: Jun 28, 2019
 *      Author: lab_espl_stud11
 */

#ifndef CODE_GHOST_H_
#define CODE_GHOST_H_

#define PAUSING 4
#define PLAYING 5
#define SINGLE_MODE 1
#define MULTI_MODE 2

#define END 502
#define LOCKED 0
#define GET_OUT 1
#define CHASE 2
#define FRIGHT 3
#define EATEN 4
#define RESET 250

struct ghost{
	unsigned char x;
	unsigned char y;
	unsigned char speed;
	unsigned char uartCheck;
	unsigned char direction;
	unsigned char state;
};

#define RED_INIT_X 112
#define RED_INIT_Y 84
#define BLUE_INIT_X 96
#define BLUE_INIT_Y 108
#define ORANGE_INIT_X 128
#define ORANGE_INIT_Y 108
#define PINK_INIT_X 112
#define PINK_INIT_Y 108
#define LOCK_TIME_BLUE 250
#define LOCK_TIME_PINK 500
#define LOCK_TIME_ORANGE 750

void ghostRedMovementCalculation();
void ghostBlueMovementCalculation();
void ghostPinkMovementCalculation();
void ghostOrangeMovementCalculation();
void ghostMultiMovementCalculation();
void ghostMultiReceive();


// Function for movement inside of the box
void Red_In_Box();
void Blue_In_Box();
void Pink_In_Box();
void Orange_In_Box();

//Get out of the box and find pacman
void Get_Out_Red();
void Get_Out_Blue();
void Get_Out_Pink();
void Get_Out_Orange();

//Chasing Pacman
void Red_Chase();
void Blue_Chase();
void Pink_Chase();
void Orange_Chase();
void Multi_Chase();

//When Pacman eats Power Pellets
void Red_Frightened();
void Blue_Frightened();
void Pink_Frightened();
void Orange_Frightened();
void Multi_Frightened();

//Ghosts are eaten
void Red_Eaten();
void Blue_Eaten();
void Pink_Eaten();
void Orange_Eaten();
void Multi_Eaten();

//OUTPUT TO SCREEN
void Draw_Red();
void Draw_Blue();
void Draw_Pink();
void Draw_Orange();
void Draw_Multi();

#endif /* CODE_GHOST_H_ */
