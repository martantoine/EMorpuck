/**
 * @file shared_var.h
 * @author Antoine Martin (martantoine@tuta.io)
 * @brief 
 * @version 0.1
 * @date 2022-04-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef SHARED_VAR
#define SHARED_VAR

#include <ch.h>
#include "constants.h"
#include "typedef.h"

extern semaphore_t position_s;
extern int8_t position_x;
extern int8_t position_y;
extern angle_t position_t;
void init_position(void);

extern semaphore_t path_s;
extern step_t path[MAX_PATH_SIZE];
extern uint8_t path_size;
void init_path(void);
void reset_path(void);

/*
 * the cell's position is stored implincitly in the address :
 * the cell of position (i,j) is accesssed with gameMap[i,j]
 */ 
extern semaphore_t gameMap_s;
extern cell_t gameMap[GAMEMAP_SIDE_NCELL][GAMEMAP_SIDE_NCELL];
void init_gameMap(void);

#endif /*SHARED_VAR*/