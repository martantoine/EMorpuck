/**
 * @file table_emo.h
 * @author Alexandre Duval(alduval0305@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef TABLE_H
#define TABLE_H

#include "constants.h"
#include "typedef.h"
#include <ch.h>

extern semaphore_t gamestates_sem;
extern uint8_t gamestates;
// this semaphore might disappear if used in only one thread, in this case this variable would become local to main()
/*
 * the cell's position is stored implincitly in the address :
 * the cell of position (i,j) is accesssed with gameMap[i,j]
 */ 
extern semaphore_t gameMap_s;
extern cell_t gameMap[GAMEMAP_SIDE_NCELL][GAMEMAP_SIDE_NCELL];
void gameMap_init(void);

//This function checks if there is still free space in the map
bool check_if_full(cell_t **gameMap);

// Checks if 3 similar signs are aligned, returns the type of the 3 alligned signs or a NO_SIGNES
line_t check_winning_condition(cell_t **gameMap);

// 2 strategy of the Epuck to place a sign in a case. Here in easy mode.
// The Epuck plays randomly, should be easy to beat it.
// The Epcuck plays randomly but it emphasis on taking the most intesting cells.
coord_t place_easy(cell_t **gameMap);
coord_t place_hard(cell_t **gameMap);

// Checks the condition of winning and losing
bool end_game(cell_t **gameMap);

void init_game(void);

#endif /*SHARED_VAR*/