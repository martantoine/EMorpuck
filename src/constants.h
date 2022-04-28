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


#endif /*CONSTANTS_H*/