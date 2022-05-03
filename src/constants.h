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

#define MAX_PATH_SIZE 30
#define GAMEMAP_SIDE_NBR_CELL   9
#define GAMEMAP_CENTER_INDEX    (GAMEMAP_SIDE_NBR_CELL - 1) / 2

#define OBSTRUCTION_BITS    ((uint8_t) (1 << 1) | (1 << 0))
#define CELL_FREE           ((uint8_t) (0 << 0))
#define CELL_OCCUPED_RED    ((uint8_t) (1 << 0))
#define CELL_OCCUPED_BLUE   ((uint8_t) (1 << 1) | (0 << 0)) 
#define PATH_FIND_BITS      ((uint8_t) (1 << 3) | (1 << 2))
#define CELL_BLANK          ((uint8_t) (0 << 3) | (0 << 2))
#define CELL_OPEN           ((uint8_t) (0 << 3) | (1 << 2))
#define CELL_CLOSED         ((uint8_t) (1 << 3) | (0 << 2))
#define CELL_UNKNOWN        ((uint8_t) (0 << 4))
#define CELL_KNOWN          ((uint8_t) (1 << 4))

#endif /*CONSTANTS_H*/