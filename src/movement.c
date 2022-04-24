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
#include "constants.h"
#include "shared_var.h"
#include <motors.h>
#include <ch.h>

#define WAIT_MOTOR_TARGET_REACHED() \
        while(left_motor_get_pos() != 0 && right_motor_get_pos() != 0) \
        chThdSleepMilliseconds(10)

void mvt_forward()
{
    //left_motor_move(CELL_WIDTH/4);
    //right_motor_move(CELL_WIDTH/4);
    WAIT_MOTOR_TARGET_REACHED();
}

void mvt_backward()
{
    //left_motor_move(-CELL_WIDTH/4);
    //right_motor_move(-CELL_WIDTH/4);
    WAIT_MOTOR_TARGET_REACHED();
}

void mvt_left()
{
    //left_motor_move(STEPS_TURN_45_DEG);
    //right_motor_move(-STEPS_TURN_45_DEG);
    WAIT_MOTOR_TARGET_REACHED();
}

void mvt_right()
{
    //left_motor_move(-STEPS_TURN_45_DEG);
    //right_motor_move(STEPS_TURN_45_DEG);
    WAIT_MOTOR_TARGET_REACHED();
}