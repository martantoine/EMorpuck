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
    printf("forward\n");
}

void mvt_backward(void)
{
    printf("backward\n");
}

void mvt_left(void)
{
    printf("left\n");
}

void mvt_right(void)
{
    printf("right\n");
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
    pathFindingReset();

    gameMap[position_x][position_y].parent = NULL; // stop code for the generatePathCode's loop
    gameMap[target_x][target_y].state |= CELL_OPEN;
    uint8_t f_min, x_min = 0, y_min = 0;
    
    for(;;) {
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
        if((x_min == position_x) && (y_min == position_y))
            break;
        
        for(uint8_t i = 0; i < 4; i++) {
            // Check boundaries
            if((x_min + neighboors[2*i]   < 0) || (x_min + neighboors[2*i]   >= GAMEMAP_SIDE_NBR_CELL) || 
               (y_min + neighboors[2*i+1] < 0) || (y_min + neighboors[2*i+1] >= GAMEMAP_SIDE_NBR_CELL)) {
            }
            // Check for obstacles and already checked cells
            else if(((gameMap[x_min + neighboors[2*i]][y_min + neighboors[2*i+1]].state & OBSTRUCTION_BITS) == CELL_OCCUPED_BLUE) ||
               ((gameMap[x_min + neighboors[2*i]][y_min + neighboors[2*i+1]].state & OBSTRUCTION_BITS) == CELL_OCCUPED_RED)  ||
               ((gameMap[x_min + neighboors[2*i]][y_min + neighboors[2*i+1]].state & PATH_FIND_BITS) == CELL_CLOSED)) {
            }
            else if((gameMap[x_min][y_min].g_score + UNIT_LENGTH < gameMap[x_min+neighboors[2*i]][y_min+neighboors[2*i+1]].g_score) ||
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

    while(icell->parent != NULL) {
        icell_old = icell;
        icell = icell_old->parent;
        printf("address=%x\n", icell);
        int32_t delta_addr = (icell - icell_old);

        switch(delta_addr) {
            case +GAMEMAP_SIDE_NBR_CELL:
                delta_x = +1;
                delta_y = 0;
                break;
            case -GAMEMAP_SIDE_NBR_CELL:
                delta_x = -1;
                delta_y = 0;
                break;
            case +1:
                delta_x = 0;
                delta_y = +1;
                break;
            case -1:
                delta_x = 0;
                delta_y = -1;
                break;    
            default:
                printf("error mtfk\n");
                break;
        }

        switch(t_current) {
            case E:
                if(delta_x == 1)
                    path[path_size++] = FORWARD;
                else if(delta_x == -1)
                    path[path_size++] = BACKWARD;
                else if(delta_y == 1) {
                    path[path_size++] = RIGHT;
                    path[path_size++] = FORWARD;
                    t_current = S;
                } 
                else if(delta_y == -1) {
                    path[path_size++] = LEFT;
                    path[path_size++] = FORWARD;
                    t_current = N;
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
                    path[path_size++] = BACKWARD;
                else if(delta_y == -1)
                    path[path_size++] = FORWARD;
                break;
            case W:
                if(delta_x == 1)
                    path[path_size++] = BACKWARD;
                else if(delta_x == -1)
                    path[path_size++] = FORWARD;
                else if(delta_y == 1) {
                    path[path_size++] = LEFT;
                    path[path_size++] = FORWARD;
                    t_current = S;
                } 
                else if(delta_y == -1) {
                    path[path_size++] = RIGHT;
                    path[path_size++] = FORWARD;
                    t_current = N;
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
                    path[path_size++] = FORWARD;
                else if(delta_y == -1)
                    path[path_size++] = BACKWARD;
                break;
        }
    }
    angle_t delta_angle = 10 + ((t_current > target_t) ? t_current - target_t : target_t - t_current);
    printf("angle = %d\n", delta_angle);
    //delta_angle = (delta_angle < 10) ? 18 +  delta_angle : delta_angle;
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
            //set to zero state's bits related to path finding
            gameMap[x][y].state = 0;
            gameMap[x][y].f_score = 0xFF;
            gameMap[x][y].g_score = 0xFF;
            //reset of parent not really necessary, test to remove
            gameMap[x][y].parent = NULL;           
        }
    reset_path();
}

uint8_t getDistance(uint8_t A_x, uint8_t A_y, uint8_t B_x, uint8_t B_y) {
    int8_t delta_x = A_x - B_x;
    int8_t delta_y = A_y - B_y;
    return (uint8_t)(10 * sqrt(delta_x*delta_x+delta_y*delta_y));
}

void updatePosition(step_t* step)
{
    int16_t delta = 0;
    if(*step == FORWARD)
        delta = 1;
    else if(*step == BACKWARD)
        delta = -1;

    if((*step == FORWARD) || (*step == BACKWARD)) {
        if(position_t == E)
            position_x += delta;
        else if(position_t == W)
            position_x -= delta;
        else if(position_t == N)
            position_y -= delta;
        else if(position_t == S)
            position_y += delta;
    }

    if(*step == LEFT) {
        switch(position_t) {
            case E: position_t = N; break;
            case N: position_t = W; break;
            case W: position_t = S; break;
            case S: position_t = E; break;
        }
    }
    else if(*step == RIGHT) {
        switch(position_t) {
            case E: position_t = S; break;
            case N: position_t = E; break;
            case W: position_t = N; break;
            case S: position_t = W; break;
        }
    }/*
    if(*step == LEFT)
        position_t = (position_t + 2 - 10) % 8 + 10;
    else if(*step == RIGHT)
        position_t = (position_t - 2 - 10) % 8 + 10;*/
}

void readpath(void)
{
    if(path != NULL)
    {
        u_int16_t i = 0;
        while(path[i] != STOP)
        {/*
            switch(path[i])
            {
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
            }*/
            updatePosition(&path[i]);
            i++;
        }
    }
}

int main(void)
{
    //(x,y)
    /* ----------------> x
     * |
     * |
     * |
     * V
     * y
     * 0  01  02  03
     * 10 11  12  13
     * 20 21  22  23
     * 30 31  32  33
     */
    int test[4][4];
    for(int x=0; x < 4; x++) {
        for(int y=0; y < 4; y++) {
            test[x][y] = x*10 + y;
        }
    }
    for(int i=0; i < 16; i++);
   //    printf("element i=%d, =%d\n", i, *(*test+i));

    init_gameMap();
    init_path();
    init_position();
    printf("position_x = %d, position_y = %d, position_t = %d\n", position_x, position_y, position_t);
    
    findPath(position_x+2, position_y, N);
    readpath();
    printf("position_x = %d, position_y = %d, position_t = %d\n", position_x, position_y, position_t);
    
    findPath(position_x-2, position_y-2, N);
    readpath();
    printf("position_x = %d, position_y = %d, position_t = %d\n", position_x, position_y, position_t);
     
    return 0;
}