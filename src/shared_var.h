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

static semaphore_t position_s;
static int8_t position_x;
static int8_t position_y;
static angle_t position_t;
void init_position(void);

enum step { FORWARD, BACKWARD, LEFT, RIGHT, STOP };
typedef enum step step_t;

static semaphore_t path_s;
static step_t* path;
void init_path(void);

#endif /*SHARED_VAR*/