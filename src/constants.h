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
#define CELL_FREE           0
#define CELL_OCCUPED_RED    1
#define CELL_OCCUPED_BLUE   2
#define CELL_CHECKED        -10 
#define CELL_2CHECK         10
#define CELL_PRIORITY       100
#define NB_SIDE_CELLS 3

enum color {
    RED,
    BLUE
};
typedef enum color color_t;

enum distnorm {
    FIRST_CASE,
    SECOND_CASE,
    THIRD_CASE,
    OUT_MAP
};
typedef enum distnorm distnorm_t;

#define LED_ON   1
#define LED_OFF 0
#define STATE_EASY 0
#define STATE_HARD 1
#define STATE_PLAYING 2
#define STATE_WAITING_FOR_PLAYER 3

#define IR_LEFT 6
#define IR_RIGHT 3

#define IMAGE_BUFFER_SIZE		640
#endif /*CONSTANTS_H*/