/*
 * Map h
 *
 *  Created on: Jun 19, 2019
 *      Author: lab_espl_stud11
 */

#ifndef CODE_MAP_H_
#define CODE_MAP_H_

#include "FreeRTOS.h"
#include "semphr.h"

#define MAZE_LEN 30
#define MAZE_WID 28
//the layout of the map

typedef struct game_map{
	char food[30][28];
	SemaphoreHandle_t lock;
}game_map_t;

extern const char maze1[30][28];
extern game_map_t *food1;
extern const char food[30][28];

void mapInit(void);

#endif /* CODE_MAP_H_ */
