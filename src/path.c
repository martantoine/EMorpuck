#include "path.h"
#include "shared_var.h"
#include <math.h>
#include <stdlib.h>

/*
 * those functions are inline because they are only used once by findPath()
 */
inline static void pathFindingReset(void);
inline static uint8_t getDistance(uint8_t A_x, uint8_t A_y, uint8_t B_x, uint8_t B_y);
inline static void generatePathCode(angle_t target_t);

/*
 * list of nearest neighboors couples (delta_x, delta_y)
 * some A star implementation allow diagonal crossing of cells
 * for sake of simplicity only horizontal and vertical crossing is possible 
 */
static int8_t neighboors[8] = {
    1 ,  0,
    -1,  0,
    0 ,  1,
    0 , -1
};

#define UNIT_LENGTH 10

void findPath(uint8_t target_x, uint8_t target_y, angle_t target_t) {
    /*
     * the pahtfinding algorithm start from the target and search the path toward the current position
     * it may seems illogical but this is way, the path obtained through the pointers "parent" starting
     * from the current position is in the right order (current_pos -> intermediate pos -> ... -> target)
     * Otherwise the path's would be reversed (target -> intermiate pos -> ... -> current_pos)
     * this later form needs further process : inverse the list, or process it backward; not handy in any case
     */ 
    gameMap[position_x][position_y].parent = NULL; // stop code for the generatePathCode's loop
    gameMap[target_x][target_y].state |= CELL_OPEN;
    uint8_t f_min, x_min = 0, y_min = 0;
    
    bool c = true;
    while(c) {
        /*
        * the default f_min value is 0xFF (0xFF)
        * considering the size of the gameMap a such value should never be obtained
        * TODO: test if uint8_t is enough for the f_score & g_score
        */
        f_min = 0xFF;
        for(uint8_t x = 0; x < GAMEMAP_SIDE_NBR_CELL; x++)
            for(uint8_t y = 0; y < GAMEMAP_SIDE_NBR_CELL; y++)
                if(((gameMap[x][y].state & PATH_FIND_BITS) == CELL_OPEN) && (gameMap[x][y].f_score <= f_min)) {
                    f_min = gameMap[x][y].f_score;
                    x_min = x;
                    y_min = y;
                }
        gameMap[x_min][y_min].state = (gameMap[x_min][y_min].state & ~PATH_FIND_BITS) | CELL_CLOSED;
        // The current position has been reached (starting from the target)
        if((x_min == position_x) && (y_min == target_y))
            c = false;
        
        for(uint8_t i = 0; i < 4; i++) {
            // Check boundaries
//            if((x_min + neighboors[2*i]   < 0) || (x_min + neighboors[2*i]   > GAMEMAP_SIDE_NBR_CELL) || 
//               (y_min + neighboors[2*i+1] < 0) || (y_min + neighboors[2*i+1] > GAMEMAP_SIDE_NBR_CELL));
    
//            else if(((gameMap[x_min + neighboors[2*i]][y_min + neighboors[2*i+1]].state & OBSTRUCTION_BITS) == CELL_OCCUPED_BLUE) ||
//               ((gameMap[x_min + neighboors[2*i]][y_min + neighboors[2*i+1]].state & OBSTRUCTION_BITS) == CELL_OCCUPED_RED)  ||
//               ((gameMap[x_min + neighboors[2*i]][y_min + neighboors[2*i+1]].state & PATH_FIND_BITS) == CELL_CLOSED));
            if((gameMap[x_min][y_min].g_score + UNIT_LENGTH < gameMap[x_min+neighboors[2*i]][y_min+neighboors[2*i+1]].g_score) ||
              ((gameMap[x_min + neighboors[2*i]][y_min+neighboors[2*i+1]].state & PATH_FIND_BITS) == CELL_BLANK))
            {
                gameMap[x_min + neighboors[2*i]][y_min+neighboors[2*i+1]].f_score =
                    gameMap[x_min][y_min].g_score + UNIT_LENGTH + getDistance(x_min + neighboors[2*i], y_min+neighboors[2*i+1], position_x, position_y);
                gameMap[x_min + neighboors[2*i]][y_min + neighboors[2*i+1]].parent = &gameMap[x_min][y_min];
                gameMap[x_min + neighboors[2*i]][y_min + neighboors[2*i+1]].state |= CELL_OPEN;
            }
        }
    }
    /*
    gameMap[position_x][position_y].parent = &gameMap[position_x+1][position_y].parent;
    gameMap[position_x+1][position_y].parent = &gameMap[position_x+2][position_y].parent;
    gameMap[position_x+2][position_y].parent = &gameMap[position_x+2][position_y+1].parent;
    gameMap[position_x+2][position_y+1].parent = &gameMap[position_x+2][position_y+2].parent;
    */
    generatePathCode(target_t);
}

void generatePathCode(angle_t target_t) {
    cell_t* icell = &gameMap[position_x][position_y];
    cell_t* icell_old = NULL;
    int8_t delta_x = 0, delta_y = 0;
    angle_t t_current = position_t;

    while(icell->parent != NULL) {
        icell_old = icell;
        
        icell = icell_old->parent;
        int32_t delta_addr = icell - icell_old;

        switch(delta_addr) {
            case +1*sizeof(cell_t):
                delta_x = +1;
                delta_y = 0;
                break;
            case -1*sizeof(cell_t):
                delta_x = -1;
                delta_y = 0;
                break;
            case +GAMEMAP_SIDE_NBR_CELL*sizeof(cell_t):
                delta_x = 0;
                delta_y = +1;
                break;
            case -GAMEMAP_SIDE_NBR_CELL*sizeof(cell_t):
                delta_x = 0;
                delta_y = -1;
                break;    
        }

        switch(t_current) {
            case E:
                if(delta_x == 1)
                    path[path_size++] = FORWARD;
                else if(delta_x == -1)
                    path[path_size++] = BACKWARD;
                else if(delta_y == 1) {
                    path[path_size++] = LEFT;
                    path[path_size++] = FORWARD;
                    t_current = N;
                } 
                else if(delta_y == -1) {
                    path[path_size++] = RIGHT;
                    path[path_size++] = FORWARD;
                    t_current = S;
                }
                break;
            case N:
                if(delta_x == 1) {
                    path[path_size++] = RIGHT;
                    path[path_size++] = FORWARD;
                    t_current = E;
                }
                else if(delta_x == -1) {
                    path[path_size++] = LEFT;
                    path[path_size++] = FORWARD;
                    t_current = W;
                }
                else if(delta_y == 1)
                    path[path_size++] = FORWARD;
                else if(delta_y == -1)
                    path[path_size++] = BACKWARD;
                break;
            case W:
                if(delta_x == 1)
                    path[path_size++] = BACKWARD;
                else if(delta_x == -1)
                    path[path_size++] = FORWARD;
                else if(delta_y == 1) {
                    path[path_size++] = RIGHT;
                    path[path_size++] = FORWARD;
                    t_current = N;
                } 
                else if(delta_y == -1) {
                    path[path_size++] = LEFT;
                    path[path_size++] = FORWARD;
                    t_current = S;
                }
                break;
            case S:
                if(delta_x == 1) {
                    path[path_size++] = LEFT;
                    path[path_size++] = FORWARD;
                    t_current = E;
                }
                else if(delta_x == -1) {
                    path[path_size++] = RIGHT;
                    path[path_size++] = FORWARD;
                    t_current = W;
                }
                else if(delta_y == 1)
                    path[path_size++] = BACKWARD;
                else if(delta_y == -1)
                    path[path_size++] = FORWARD;
                break;
        }
    }
    angle_t delta_angle = t_current - target_t;
    delta_angle = (delta_angle < 10) ? 18 +  delta_angle : delta_angle;
    switch(delta_angle) {
        case E:
            break;
        case N:
            path[path_size++] = LEFT;
            break;
        case W:
            path[path_size++] = LEFT;
            path[path_size++] = LEFT;
            break;
        case S:
            path[path_size++] = RIGHT;
            break;
    }
}

void pathFindingReset(void) {
    for(uint8_t x = 0; x < GAMEMAP_SIDE_NBR_CELL; x++)
        for(uint8_t y = 0; y < GAMEMAP_SIDE_NBR_CELL; y++) {
            gameMap[x][y].f_score = 0xFF;
            //reset of parent not really necessary, test to remove
            gameMap[x][y].parent = NULL;
            //set to zero state's bits related to path finding
            gameMap[x][y].state &= ~PATH_FIND_BITS;
        }
    reset_path();
}

uint8_t getDistance(uint8_t A_x, uint8_t A_y, uint8_t B_x, uint8_t B_y) {
    uint8_t delta_x = A_x - B_x;
    uint8_t delta_y = A_y - B_y;
    return (uint8_t)(10 * sqrt(delta_x*delta_x+delta_y*delta_y));
}