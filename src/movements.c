#include <ch.h>
#include <stdlib.h>
#include "movements.h"
#include "motors_driver.h"
#include "shared_var.h"
#include "constants.h"

inline static void updatePosition(coord_t *position, step_t step); //inline because used only in one place : 1 line in mvt_executablePath
static void calibrationPosition(void);

void mvt_init(void) {
    motors_init();
    right_motor_set_speed(400);
    left_motor_set_speed(400);
    calibrationPosition();
}

void mvt_executePath(coord_t *position, step_t *path) {
    if(!path) {
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
            updatePosition(position, path[i]);
            i++;
        }
        free(path); //better not forget to free unused memory
    }
    else
        chSysHalt("path null");
}

static void updatePosition(coord_t *position, step_t step) {
    int8_t delta = 0;
    if(step == FORWARD)
        delta = 1;
    else if(step == BACKWARD)
        delta = -1;

    if((step == FORWARD) || (step == BACKWARD)) {
        if(position->t == E)
            position->x += delta;
        else if(position->t == W)
            position->x -= delta;
        else if(position->t == N)
            position->y -= delta;
        else if(position->t == S)
            position->y += delta;
    }

    if(step == LEFT) {
        switch(position->t) {
            case E: position->t = N; break;
            case N: position->t = W; break;
            case W: position->t = S; break;
            case S: position->t = E; break;
        }
    }
    else if(step == RIGHT) {
        switch(position->t) {
            case E: position->t = S; break;
            case N: position->t = E; break;
            case W: position->t = N; break;
            case S: position->t = W; break;
        }
    }
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