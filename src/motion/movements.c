#include <ch.h>
#include <stdlib.h>
#include "movements.h"
#include "motors_driver.h"
#include "path.h"

void mvt_init(void) {
    motors_init();
    right_motor_set_speed(STEP_SPEED);
    left_motor_set_speed(STEP_SPEED);
}

void mvt_executePath(coord_t *position, const coord_t target, step_t *path) {
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
        i++;
    }
    *position = target;
    free(path); //better not forget to free unused memory
}

void mvt_place(cell_t gameMap[SIDE_NCELL][SIDE_NCELL], coord_t *position, const coord_t target) {
    coord_t min, tmp;
    uint8_t min_dist = 0xFF, i;

    // find among all available pieces the cloest one to the target
    for(i = 0; i < 12; i++) {
        tmp = storage[i];
        if((gameMap[tmp.x][tmp.y].state & OBSTRUCTION_BITS) == CELL_OCCUPED_RED) {
            uint8_t dist = getDistance((coord_t){.x=tmp.x, .y=tmp.y, .t=tmp.t}, target);
            if(dist <= min_dist) {
                min = pickup[i];
                min_dist = dist;
            }
        }
    }
    // go behind the stored piece (in the pickup position)
    step_t *path = findPath(gameMap, *position, min);
    mvt_executePath(position, min, path);
    // go forward 4*(1/2 cell) = 2 cells => is now between the storage ring and the 3x3 playing grid
    mvt_forward();
    mvt_forward();
    mvt_forward();
    mvt_forward();
    switch(position->t) {
        case E: position->x += 2; break;
        case W: position->x -= 2; break;
        case N: position->y -= 2; break;
        case S: position->y += 2; break;
    }
    gameMap[storage[i].x][storage[i].y].state &= ~OBSTRUCTION_BITS; // the robot now see the cell free and can go through it
    // search the place to place the piece
    path = findPath(gameMap, *position, target);
    mvt_executePath(position, target, path);
    // go back to idling positon
    path = findPath(gameMap, *position, (coord_t){1, 7, N});
    mvt_executePath(position, (coord_t){1, 7, N}, path);
    // mark the cell that received the red piece
    gameMap[target.x][target.y].state = (gameMap[target.x][target.y].state & ~OBSTRUCTION_BITS) | CELL_OCCUPED_RED;
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