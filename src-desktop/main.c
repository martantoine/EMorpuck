#include <math.h>
#include <stdlib.h>
#include <stdio.h>

typedef u_int8_t uint8_t;
typedef int8_t int8_t;

uint8_t getDistance(uint8_t A_x, uint8_t A_y, uint8_t B_x, uint8_t B_y);

#define MAX_PATH_SIZE 30
#define GAMEMAP_SIDE_NBR_CELL   9
#define GAMEMAP_CENTER_INDEX    ((GAMEMAP_SIDE_NBR_CELL - 1) / 2)

enum angle { 
    E=10,
    N=12,
    W=14,
    S=16,
};
typedef enum angle angle_t;

int8_t position_x;
int8_t position_y;
angle_t position_t;

enum step { FORWARD, BACKWARD, LEFT, RIGHT, STOP };
typedef enum step step_t;

step_t path[MAX_PATH_SIZE];
uint8_t path_size;

#define UNIT_LENGTH 10
#define OBSTRUCTION_BITS   ((uint8_t) (1 << 1) | (1 << 0))
#define CELL_FREE          ((uint8_t) (0 << 0))
#define CELL_OCCUPED_RED   ((uint8_t) (1 << 0))
#define CELL_OCCUPED_BLUE  ((uint8_t) (1 << 1) | (0 << 0))
#define PATH_FIND_BITS     ((uint8_t) (1 << 3) | (1 << 2))
#define CELL_BLANK         ((uint8_t) (0 << 3) | (0 << 2))
#define CELL_OPEN          ((uint8_t) (0 << 3) | (1 << 2))
#define CELL_CLOSED        ((uint8_t) (1 << 3) | (0 << 2))
#define CELL_UNKNOWN       ((uint8_t) (0 << 4))
#define CELL_KNOWN         ((uint8_t) (1 << 4))
struct cell
{
    uint8_t state;
    uint8_t f_score, g_score; 
    struct cell* parent;
};
typedef struct cell cell_t;
cell_t gameMap[GAMEMAP_SIDE_NBR_CELL][GAMEMAP_SIDE_NBR_CELL];

void init_position()
{
    position_x = GAMEMAP_CENTER_INDEX;
    position_y = GAMEMAP_CENTER_INDEX;
    position_t = N;
}

void reset_path(void);

void init_path()
{
    reset_path();
}

void reset_path(void)
{
    for(uint8_t i = 0; i < MAX_PATH_SIZE; i++)
        path[i] = STOP;
    path_size = 0;
}

void init_gameMap(void)
{   
    for(uint8_t i = 0; i < GAMEMAP_SIDE_NBR_CELL; i++)
        for(uint8_t j = 0; j < GAMEMAP_SIDE_NBR_CELL; j++)
        {
            gameMap[i][j] = (cell_t){
                .state = CELL_BLANK,
                .f_score = 0xFF,        // 0xFF is large enough to be always be greater than the one calculated 
                .parent = NULL
            };
            gameMap[i][j].state = CELL_BLANK;
        }
}

void mvt_forward(void)
{
    printf("forward");
}

void mvt_backward(void)
{
    printf("backward");
}

void mvt_left(void)
{
    printf("left");
}

void mvt_right(void)
{
    printf("right");
}

inline void pathFindingReset(void);
inline uint8_t getDistance(uint8_t A_x, uint8_t A_y, uint8_t B_x, uint8_t B_y);
void generatePathCode(angle_t target_t);

static int8_t neighboors[8] = {
    1 ,  0,
    -1,  0,
    0 ,  1,
    0 , -1
};

void findPath(uint8_t target_x, uint8_t target_y, angle_t target_t) {
    gameMap[position_x][position_y].parent = NULL; // stop code for the generatePathCode's loop
    gameMap[target_x][target_y].state |= CELL_OPEN;
    gameMap[target_x][target_y].g_score = 0;
    uint8_t f_min, x_min = 0, y_min = 0;
    int i = 0;
    while(1==1) {
        f_min = 0xFF;
        for(uint8_t x = 0; x < GAMEMAP_SIDE_NBR_CELL; x++) {
            for(uint8_t y = 0; y < GAMEMAP_SIDE_NBR_CELL; y++) {
                if(((gameMap[x][y].state & PATH_FIND_BITS) == CELL_OPEN) & (gameMap[x][y].f_score <= f_min)) {
                    f_min = gameMap[x][y].f_score;
                    x_min = x;
                    y_min = y;
                }
            }
        }
        
        gameMap[x_min][y_min].state = (gameMap[x_min][y_min].state & ~PATH_FIND_BITS) | CELL_CLOSED;
        // The current position has been reached (starting from the target)
        if((x_min == position_x) && (y_min == target_y))
            break;
        for(uint8_t i = 0; i < 4; i++) {
            if((gameMap[x_min][y_min].g_score + UNIT_LENGTH < gameMap[x_min+neighboors[2*i]][y_min+neighboors[2*i+1]].g_score) &&
              ((gameMap[x_min + neighboors[2*i]][y_min+neighboors[2*i+1]].state & PATH_FIND_BITS) == CELL_BLANK))
            {
                gameMap[x_min + neighboors[2*i]][y_min+neighboors[2*i+1]].g_score = gameMap[x_min][y_min].g_score + UNIT_LENGTH;
                gameMap[x_min + neighboors[2*i]][y_min+neighboors[2*i+1]].f_score =
                    gameMap[x_min + neighboors[2*i]][y_min+neighboors[2*i+1]].g_score + getDistance(x_min + neighboors[2*i], y_min+neighboors[2*i+1], position_x, position_y);
                
                gameMap[x_min + neighboors[2*i]][y_min + neighboors[2*i+1]].parent = &gameMap[x_min][y_min];
                gameMap[x_min + neighboors[2*i]][y_min + neighboors[2*i+1]].state = 
                    (gameMap[x_min + neighboors[2*i]][y_min + neighboors[2*i+1]].state & ~PATH_FIND_BITS) | CELL_OPEN;
            }
        }
    }
    generatePathCode(target_t);
}

void generatePathCode(angle_t target_t) {
    cell_t* icell = &gameMap[position_x][position_y];
    cell_t* icell_old = NULL;
    int8_t delta_x = 0, delta_y = 0;
    angle_t t_current = position_t;
    printf("sizeof(cell_t) = %d\n", sizeof(cell_t));
    printf("sizeof(cell_t) = %d\n", 8%8);

    while(icell->parent != NULL) {
        icell_old = icell;
        
        icell = icell_old->parent;
        int delta_addr = icell - icell_old;
        //printf("x = %d, y = %d\n", x_min, y_min);
        printf("daddr = %d\n", delta_addr);

        if(delta_addr == +1) {
            delta_x = +1;
            delta_y = 0;
        }
        else if(delta_addr == -1) {
            delta_x = -1;
            delta_y = 0;
        }
        else if(delta_addr == GAMEMAP_SIDE_NBR_CELL) {
            delta_x = 0;
            delta_y = +1;
        }
        else if(delta_addr == -GAMEMAP_SIDE_NBR_CELL) {
            delta_x = 0;
            delta_y = -1;
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
    for(size_t i=0; i < path_size;i++)
        if(path[i] == FORWARD)
            printf("path i=%d, code=FORWARD\n", i);
        else if(path[i] == BACKWARD)
            printf("path i=%d, code=BACKWARD\n", i);
        else if(path[i] == LEFT)
            printf("path i=%d, code=LEFT\n", i);
        else if(path[i] == RIGHT)
            printf("path i=%d, code=RIGHT\n", i);
}

void pathFindingReset(void) {
    for(uint8_t x = 0; x < GAMEMAP_SIDE_NBR_CELL; x++)
        for(uint8_t y = 0; y < GAMEMAP_SIDE_NBR_CELL; y++) {
            gameMap[x][y].f_score = 0xFF;
            gameMap[x][y].g_score = 0xFF;
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
    return (uint8_t)(10 * (delta_x*delta_x+delta_y*delta_y));
}

int main(void)
{
    init_gameMap();
    init_path();
    init_position();
    pathFindingReset();
    findPath(position_x + 2, position_y - 2, N);
    return 0;
}