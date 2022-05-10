#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <stdint.h>

/*
 * Differences between angles are 2 to able the possible extension toward 4 more directions :
 * NE, NW, SE, SW
 */
enum angle {
    E=10,
    N=12,
    W=14,
    S=16,
};
typedef enum angle angle_t;

enum step { STOP=0, FORWARD, BACKWARD, LEFT, RIGHT };
typedef enum step step_t;

struct cell {
    /*
     * the state variable store all the different states of the cell :
     * bit 0-1 : free = 00 / occuped red = 01 / occuped blue = 10
     * bit 2-3 : blank = 00 / open = 01 / closed = 10
     * bit 4   : unknown = 0 / known = 1
     */
    uint8_t state;
    /*
     * f_score = g_score + H_score,
     * g_score : distance travelled from the origin cell
     * h_score : estimated minimal distance to the destination cell
     */
    uint8_t f_score, g_score;
    struct cell* parent;
};
typedef struct cell cell_t;

struct coord {
    uint8_t x,y;
    angle_t t;
};
typedef struct coord coord_t;

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

enum line{
    BLUE_LINE,
    RED_LINE,
    NO_LINE
};
typedef enum line line_t;

#endif /*TYPEDEF_H*/