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

#include <ch.h>
#include "movement.h"
#include "motors_driver.h"
#include "constants.h"

#define WAIT_MOTOR_TARGET_REACHED() \
    while((left_motor_steps_left() != 0) || (right_motor_steps_left() != 0));
        //chThdSleepMilliseconds(10)

void mvt_forward(void)
{
    left_motor_move(CELL_WIDTH/4);
    right_motor_move(CELL_WIDTH/4);
    WAIT_MOTOR_TARGET_REACHED();
}

void mvt_backward(void)
{
    left_motor_move(-CELL_WIDTH/4);
    right_motor_move(-CELL_WIDTH/4);
    WAIT_MOTOR_TARGET_REACHED();
}

void mvt_left(void)
{
    left_motor_move(STEPS_TURN_45_DEG);
    right_motor_move(-STEPS_TURN_45_DEG);
    WAIT_MOTOR_TARGET_REACHED();
}

void mvt_right(void)
{
    left_motor_move(-STEPS_TURN_45_DEG);
    right_motor_move(STEPS_TURN_45_DEG);
    WAIT_MOTOR_TARGET_REACHED();
}