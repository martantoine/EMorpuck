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

#include <stdint.h>
#include <ch.h>
#include "constants.h"

enum angle { 
    E=0,
    NE=1,
    N=2,
    NW=3,
    W=4,
    SW=4,
    S=6,
    SE=7
};
typedef enum angle angle_t;

extern semaphore_t position_s;
extern int8_t position_x;
extern int8_t position_y;
extern angle_t position_t;
void init_position(void);

enum step { FORWARD, BACKWARD, LEFT, RIGHT, STOP };
typedef enum step step_t;

extern semaphore_t path_s;
extern memory_heap_t* path_h;
extern step_t* path;
void init_path(void);

struct cell
{
    uint8_t state;
    uint8_t f_score;
    struct cell* parent;
};
typedef struct cell cell_t;

extern semaphore_t gameMap_s;
extern cell_t gameMap[GAMEMAP_SIDE_NBR_CELL][GAMEMAP_SIDE_NBR_CELL];
void init_gameMap(void);

#endif /*SHARED_VAR*/