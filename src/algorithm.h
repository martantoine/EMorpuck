/**
 * @file algorithm.h
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

#include <stdio.h>
#include <stdint.h>
#include <ch.h>
#include <time.h>

#include "constants.h"
#include "table.h"



void place_easy(uint8_t * table[NB_SIDE_CELLS][NB_SIDE_CELLS]);

void place_hard();


void end_game();
#endif /*SHARED_VAR*/