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

#include "../defs.h"

void mvt_init(void);
void mvt_executePath(coord_t *position, step_t *path);
void mvt_place(cell_t gameMap[SIDE_NCELL][SIDE_NCELL], coord_t *position, const coord_t target);

void mvt_forward(void);
void mvt_backward(void);
void mvt_left(void);
void mvt_right(void);

#endif /*MOVEMENTS_H*/
