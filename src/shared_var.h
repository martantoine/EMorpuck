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

/*
 * Differences between angles are 2 to able the possible extension toward 4 more directions :
 * NE, NW, SE, SW
 */
enum angle { 
    E=10,
    N=12,
    W=14,
    S=16,
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
extern step_t path[MAX_PATH_SIZE];
extern uint8_t path_size;
void init_path(void);
void reset_path(void);

struct cell {
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
extern cell_t gameMap[GAMEMAP_SIDE_NCELL][GAMEMAP_SIDE_NCELL];
void init_gameMap(void);

#endif /*SHARED_VAR*/