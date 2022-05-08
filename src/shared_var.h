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

extern const coord_t nearest[4];

// this semaphore might disappear if used in only one thread, in this case this variable would become local to main()
/*
 * the cell's position is stored implincitly in the address :
 * the cell of position (i,j) is accesssed with gameMap[i,j]
 */ 
extern semaphore_t gameMap_s;
extern cell_t gameMap[GAMEMAP_SIDE_NCELL][GAMEMAP_SIDE_NCELL];
void init_gameMap(void);

#endif /*SHARED_VAR*/