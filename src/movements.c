#include <ch.h>
#include "movements.h"
#include "motors_driver.h"
#include "shared_var.h"
#include "constants.h"

inline static void updatePosition(step_t* step); //inline because used only in one place
static void calibrationPosition(void);

void mvt_executePath(void) {
    uint8_t i = 0;
    while(path[i] != STOP) {
        switch(path[i]) {
            case FORWARD: 
                mvt_forward();
                break;
            case BACKWARD:
                mvt_backward();
                break;
            case LEFT: 
                mvt_left();
                break;
            case RIGHT:
                mvt_right();
                break;
            default:
                break;
        }
        updatePosition(&path[i]);
        i++;
    }
}

static void updatePosition(step_t* step) {
    int8_t delta = 0;
    if(*step == FORWARD)
        delta = 1;
    else if(*step == BACKWARD)
        delta = -1;

    if((*step == FORWARD) || (*step == BACKWARD)) {
        if(position_t == E)
            position_x += delta;
        else if(position_t == W)
            position_x -= delta;
        else if(position_t == N)
            position_y -= delta;
        else if(position_t == S)
            position_y += delta;
    }

    if(*step == LEFT) {
        switch(position_t) {
            case E: position_t = N; break;
            case N: position_t = W; break;
            case W: position_t = S; break;
            case S: position_t = E; break;
        }
    }
    else if(*step == RIGHT) {
        switch(position_t) {
            case E: position_t = S; break;
            case N: position_t = E; break;
            case W: position_t = N; break;
            case S: position_t = W; break;
        }
    }
}

void mvt_init(void) {
    motors_init();
    right_motor_set_speed(400);
    left_motor_set_speed(400);
    calibrationPosition();
}

void calibrationPosition(void) {
    /*
     * Do some obscure things to align the robot with the game map
     */
}

#define WAIT_MOTOR_TARGET_REACHED() \
    while((left_motor_steps_left() != 0) || (right_motor_steps_left() != 0)) \
        chThdSleepMilliseconds(100)

void mvt_forward(void) {
    left_motor_move(NSTEPS_HALF_CELL);
    right_motor_move(NSTEPS_HALF_CELL);
    WAIT_MOTOR_TARGET_REACHED();
}

void mvt_backward(void) {
    left_motor_move(-NSTEPS_HALF_CELL);
    right_motor_move(-NSTEPS_HALF_CELL);
    WAIT_MOTOR_TARGET_REACHED();
}

void mvt_left(void) {
    left_motor_move(-STEPS_TURN_90);
    right_motor_move(STEPS_TURN_90);
    WAIT_MOTOR_TARGET_REACHED();
}

void mvt_right(void) {
    left_motor_move(STEPS_TURN_90);
    right_motor_move(-STEPS_TURN_90);
    WAIT_MOTOR_TARGET_REACHED();
}