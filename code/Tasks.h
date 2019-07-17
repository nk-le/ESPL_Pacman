/*
 * pac_man.h
 *
 *  Created on: Jun 28, 2019
 *      Author: lab_espl_stud11
 */

#ifndef CODE_TASKS_H_
#define CODE_TASKS_H_

void pac_man(void * params);
void pacmanMulti(void * params);
void drawMap(void * param);
void resetFoodMap();
void loadImages();
void updateScreen(void * params);
void uartReceive();
void uartUpdatePacmanStatus();
void uartUpdateGhostStatus();

#endif /* CODE_TASKS_H_ */
