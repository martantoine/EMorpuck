#include "path.h"
#include "shared_var.h"
#include <math.h>
#include <stdlib.h>

//static void adaptPathFormat(_t* target);
//inline static uint8_t getDistance(node_t* A, node_t* B);
static cell_t* open;

struct coord {
    uint8_t x, y;
};
typedef struct coord coord_t;

#define OCCUPED 3
#define BLANK 2
#define OPEN 1
#define CLOSED 0

static uint8_t neighboors[8] = {
    1 ,  0,
    -1,  0,
    0 ,  1,
    0 , -1
};

void findPath(uint8_t target_x, uint8_t target_y, angle_t target_t)
{
    //TODO: resetParents();
    gameMap[target_x][target_y].state = OPEN;
    uint8_t f_min, i_min, j_min;
    
    while(true)
    {
        f_min = 0xFF;
        for(uint8_t i = 0; i < GAMEMAP_SIDE_NBR_CELL; i++)
            for(uint8_t j = 0; j < GAMEMAP_SIDE_NBR_CELL; j++)
                if((gameMap[i][j].state == OPEN) && (gameMap[i][j].f_score <= f_min))
                {
                    f_min = gameMap[i][j].f_score;
                    i_min = i;
                    j_min = j;
                }
        gameMap[i_min][j_min].state = CLOSED;
        if((i_min == target_x) && (j_min == target_y))
            break;
        for(uint8_t i = 0; i < 4; i++)
        {
            if((i_min + neighboors[2*i]   < 0)  || (i_min + neighboors[2*i]   > GAMEMAP_SIDE_NBR_CELL) || 
               (j_min + neighboors[2*i+1] < 0)  || (j_min + neighboors[2*i+1] > GAMEMAP_SIDE_NBR_CELL))
                break;
            if((gameMap[i_min+neighboors[2*i]][j_min+neighboors[2*i+1]].state == OCCUPED) ||
               (gameMap[i_min+neighboors[2*i]][j_min+neighboors[2*i+1]].state == CLOSED))
                break;
            if((gameMap[i_min][j_min].f_score + 10 < gameMap[i_min+neighboors[2*i]][j_min+neighboors[2*i+1]].f_score) ||
               (gameMap[i_min+neighboors[2*i]][j_min+neighboors[2*i+1]].state == BLANK))
            {
                gameMap[i_min+neighboors[2*i]][j_min+neighboors[2*i+1]].f_score = gameMap[i_min][j_min].f_score + 10;
                gameMap[i_min+neighboors[2*i]][j_min+neighboors[2*i+1]].parent = &gameMap[i_min][j_min];
                gameMap[i_min+neighboors[2*i]][j_min+neighboors[2*i+1]].state = OPEN;
            }

        }
    }
    /*
    uint8_t i = 0, j = 0;
    while((*nodeMap[i][j]->x != target_x) && (nodes[i].y != target_y))
    {
        //uint8_t f_score = getDistance(&nodes[i], &nodes[0]) + getDistance(&nodes[i], &target);
        //nodes[i].f_score = f_score;
    }
    */
}

void adaptPathFormat(node_t* target)
{
    /*node_t* current_node = target;
    while(current_node->parent != NULL)
    {

        current_node = current_node->parent;
    }
    free(nodes);
    */
}

/**
 * @brief Get the Distance between node A and B, heavy approximations
 * 
 * @param A 
 * @param B 
 * @return uint8_t the distance
 */
static uint8_t getDistance(node_t* A, node_t* B)
{
    uint8_t delta_x = abs(A->x - B->x);
    uint8_t delta_y = abs(A->y - B->y);
    return (uint8_t)(sqrt(delta_x^2+delta_y^2));
}