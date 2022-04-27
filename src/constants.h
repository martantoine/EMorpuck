#ifndef CONSTANTS_H
#define CONSTANTS_H

#define PI 3.14
#define WHEEL_PERIMETER 130
#define WHEEL_SPACE 50
#define NSTEP_ONE_TURN 1000 // number of step for 1 turn of the motor
#define MM_TO_STEPS (NSTEP_ONE_TURN * 2 / WHEEL_PERIMETER)
#define CELL_WIDTH 400
#define STEPS_CELL_WIDTH CELL_WIDTH * MM_TO_STEPS
#define STEPS_TURN_45_DEG (NSTEP_ONE_TURN * WHEEL_SPACE * PI / WHEEL_PERIMETER / 4)

#define DEFAULT_NBR_STEPS_IN_PATH 30
#define GAMEMAP_SIDE_NBR_CELL   9
#define GAMEMAP_CENTER_INDEX    (GAMEMAP_SIDE_NBR_CELL - 1) / 2
// the state is stored on 8 bits
/* 0 : free/occuped  
 * 1 : color
 * 2 : checked/2check
 * 3 : reserved
 * 4 : open
 * 5
 * 6
 * 7
#define CELL_FREE           0b1    // 0b0 : free, 0b1 : occuped
#define CELL_OCCUPED_RED    1    // 0b0
#define BIT_CELL_OCCUPED_BLUE   2
#define BIT_CELL_CHECKED        -10 
#define BIT_CELL_2CHECK         10
#define BIT_CELL_OPEN
#define BIT_CELL_CLOSED

#endif /*CONSTANTS_H*/