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
#include <stdint.h>
#include <ch.h>
#include "constants.h"


uint8_t Game_Table[NB_SIDE_CELLS][NB_SIDE_CELLS];

/* 
fonctions to modify the table
*/
void init_table(uint8_t * table[NB_SIDE_CELLS][NB_SIDE_CELLS]);
void add_signe(uint8_t type, uint8_t lin, uint8_t col, uint8_t * table[NB_SIDE_CELLS][NB_SIDE_CELLS]);
void change_state_checking(uint8_t state, uint8_t lin, uint8_t col, uint8_t * table[NB_SIDE_CELLS][NB_SIDE_CELLS]);
/*
    this function checks if there is still free space in the map
*/
bool check_if_full(uint8_t * table[NB_SIDE_CELLS][NB_SIDE_CELLS]);



/* 
    *This function checks if 3 similar signs are aligned 

 */ 
bool check_if_3_alligned(uint8_t * table[NB_SIDE_CELLS][NB_SIDE_CELLS]);
/*
    this function is the strategy of the Epuck to place a sign in a case. Here in easy mode.
    The Epuck plays randomly, it should be easy to beat it.
*/
void place_easy(uint8_t * table[NB_SIDE_CELLS][NB_SIDE_CELLS]);
/* 
    this function is another strategy of the Epuck to place a sign in a case. Here in hard mode.
    The Epcuck plays randomly but it emphasis on taking the powerful cells. 
*/
void place_hard(uint8_t * table[NB_SIDE_CELLS][NB_SIDE_CELLS]);

/*
checks the condition of winning and losing
*/
bool end_game(uint8_t * table[NB_SIDE_CELLS][NB_SIDE_CELLS])
{
    
}

#endif /*SHARED_VAR*/
#endif /*SHARED_VAR*/