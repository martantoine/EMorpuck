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
    N=2,
    W=4,
    S=6,
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
extern uint32_t path_alloc_size;
void init_path(void);

#define OBSTRUCTION_BITS    (1 << 1) | (1 << 0)
#define CELL_FREE           (0 << 0)    
#define CELL_OCCUPED_RED    (1 << 0)
#define CELL_OCCUPED_BLUE   (1 << 1) | (0 << 0) 
#define PATH_FIND_BITS      (1 << 3) | (1 << 2)
#define CELL_BLANK          (0 << 3) | (0 << 2)
#define CELL_OPEN           (0 << 3) | (1 << 2)
#define CELL_CLOSED         (1 << 3) | (0 << 2)
#define CELL_UNKNOWN        (0 << 4)
#define CELL_KNOWN          (1 << 4)
struct cell
{
    /*
     * the state variable store all the different states of the cell :
     * bit 0-1 : free = 00 / occuped red = 01 / occuped blue = 10
     * bit 2-3 : blank = 00 / open = 01 / closed = 10
     * bit 4   : unknown = 0 / known = 1 
     */
    uint8_t state;
    /*
     * f_score = g_score + H_score,  
     * g_score : distance travelled from the origin cell
     * h_score : estimated minimal distance to the destination cell
     */
    uint8_t f_score, g_score; 
    struct cell* parent;
};
typedef struct cell cell_t;
extern semaphore_t gameMap_s;
/*
 * the cell's position is stored implincitly in the address :
 * the cell of position (i,j) is accesssed with gameMap[i,j]
 */ 
extern cell_t gameMap[GAMEMAP_SIDE_NBR_CELL][GAMEMAP_SIDE_NBR_CELL];
void init_gameMap(void);

#endif /*SHARED_VAR*/