#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX_PATH_SIZE       30
#define GAMEMAP_SIDE_NCELL  9
#define GAMEMAP_CENTER      ((GAMEMAP_SIDE_NCELL - 1) / 2)
#define CELL_WIDTH          100 // in mm

#define NSTEP_ONE_TURN      1000 // number of full steps for 1 turn of the motor
#define PI                  3.14159
#define WHEEL_DIAMETER      41 // in mm
#define WHEEL_SPACE         53 // in mm
/*
 * The 3 following defines are commented to show the derivation of of NTURNS_TURN_360
 */
//#define SPACE_PERIMETER (WHEEL_SPACE * PI)
//#define WHEEL_PERIMETER (WHEEL_DIAMETER * PI)
//#define NTURNS_TURN_360 (SPACE_PERIMETER / WHEEL_PERIMETER)
#define NTURNS_TURN_360     (WHEEL_SPACE / WHEEL_DIAMETER)
#define STEPS_TURN_360      (NTURNS_TURN_360 * NSTEP_ONE_TURN)
#define STEPS_TURN_90       (STEPS_TURN_360 / 4)

#define NSTEPS_HALF_CELL    (CELL_WIDTH / (WHEEL_DIAMETER * PI) * NSTEP_ONE_TURN / 2)

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