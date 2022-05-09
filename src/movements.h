/**
 * @file movements.h
 * @author Antoine Martin (martantoine@tuta.io)
 * @brief 
 * @version 0.1
 * @date 2022-04-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MOVEMENTS_H
#define MOVEMENTS_H

#include "typedef.h"

void mvt_init(void);
void mvt_executePath(coord_t *position, step_t *path);

void mvt_forward(void);
void mvt_backward(void);
void mvt_left(void);
void mvt_right(void);

#endif /*MOVEMENTS_H*/
