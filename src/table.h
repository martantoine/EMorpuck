/**
 * @file table.h
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
#include "constants.h" //to be removed once Game_Table deleted

//this thing is already defined in shared_var.h
uint8_t Game_Table[GAMEMAP_SIDE_NCELL][GAMEMAP_SIDE_NCELL];

void init_table(uint8_t **table);
void add_signe(uint8_t type, uint8_t lin, uint8_t col, uint8_t **table);
void change_state_checking(uint8_t state, uint8_t lin, uint8_t col, uint8_t **table);
bool check_if_full(uint8_t **table);
void change_priority_placement(uint8_t state, uint8_t lin, uint8_t col, uint8_t **table);
bool check_if_2_alligned(uint8_t **table); 
/* This function checks if a line or a diagonal has 2 similar signs and returns if yes or no.
It also modifies the state of priority placement
 */ 
#endif /*SHARED_VAR*/