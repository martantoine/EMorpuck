/**
 * @file movement.c
 * @author Antoine Martin (martantoine@tuta.io)
 * @brief 
 * @version 0.1
 * @date 2022-04-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "movement.h"
#include <motors.h>

#define CELL_STEPS 1000;
static struct pos_t current_pos;

void initPos()
{
    current_pos.x = 0;
    current_pos.y = 0;
    current_pos.t = 0;
}

void move(struct pos_t target)
{
    uint8_t size;
    struct pos_t* path = findPath(target, &size);
    struct pos_t delta_path;
    for(uint8_t i=0; i < size; i++)
    {
        delta_path.x = path[i].x - current_pos.x;
        delta_path.y = path[i].y - current_pos.y;
        delta_path.t = path[i].t - current_pos.t;
        turn(delta_path.t);
        if(delta_path.x != 0 || delta_path.y != 0)
        {
            robot_forward();
        }
    }
}