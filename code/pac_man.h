/*
 * pac_man.h
 *
 *  Created on: Jul 2, 2019
 *      Author: lab_espl_stud11
 */

#ifndef CODE_PAC_MAN_H_
#define CODE_PAC_MAN_H_

#define GHOST_EATEN 2
#define PAC_MAN_DIED 1
#define DRAW_CHERRY 250
#define DRAW_STRAWBERRY 251
#define DRAW_ORANGE 252
#define RESETFOOD 253
#define NORMAL 0
#define POWERED 1

#define GO_LEFT 0
#define GO_RIGHT 1
#define GO_UP 2
#define GO_DOWN 3
#define GO_NOWHERE 4

struct pacman{
	unsigned char x;
	unsigned char y;
	unsigned char status;
	unsigned char direction;
	unsigned char uartCheck;
	unsigned char life;
};

#endif /* CODE_PAC_MAN_H_ */
