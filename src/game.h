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

#include "typedef.h"

/*
 * this function checks if there is still free space in the map
 */
bool check_if_full(cell_t **gameMap);

/*
 * This function checks if 3 similar signs are aligned
 * Returns the type of the 3 alligned signs or a NO_SIGNES
 */
line_t check_winning_condition(cell_t **gameMap);

/*
 * this function is the strategy of the Epuck to place a sign in a case. Here in easy mode.
 * The Epuck plays randomly, it should be easy to beat it.
 */
void place_easy(cell_t **gameMap);

/*
 * this function is another strategy of the Epuck to place a sign in a case. Here in hard mode.
 * The Epcuck plays randomly but it emphasis on taking the powerful cells. 
 */
void place_hard(cell_t **gameMap);

/*
 * checks the condition of winning and losing
 */
bool end_game(cell_t **gameMap);

void init_game(void);
uint8_t get_difficulty(void);
uint8_t get_state(void);

#endif /*SHARED_VAR*/