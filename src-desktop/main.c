#include <math.h>
#include <stdlib.h>
#include <stdio.h>

typedef u_int8_t uint8_t;
typedef u_int16_t uint16_t;

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
    uint8_t state;
    uint8_t f_score, g_score;
    struct cell* parent;
};
typedef struct cell cell_t;

struct coord {
    uint8_t x,y;
    angle_t t;
};
typedef struct coord coord_t;

void mvt_forward();
void mvt_backward();
void mvt_left();
void mvt_right();
#define MAX_PATH_SIZE               30
#define SIDE_NCELL                  9
#define GAMEMAP_CENTER              ((SIDE_NCELL - 1) / 2)
#define CELL_WIDTH                  100 // in mm

#define OBSTRUCTION_BITS            ((uint8_t) (1 << 1) | (1 << 0))
#define CELL_FREE                   ((uint8_t) (0 << 1) | (0 << 0))
#define CELL_OCCUPED                ((uint8_t) (0 << 1) | (1 << 0))
#define CELL_OCCUPED_RED            ((uint8_t) (1 << 1) | (0 << 0))
#define CELL_OCCUPED_BLUE           ((uint8_t) (1 << 1) | (1 << 0))
#define PATH_FIND_BITS              ((uint8_t) (1 << 3) | (1 << 2))
#define CELL_BLANK                  ((uint8_t) (0 << 3) | (0 << 2))
#define CELL_OPEN                   ((uint8_t) (0 << 3) | (1 << 2))
#define CELL_CLOSED                 ((uint8_t) (1 << 3) | (0 << 2))
#define CELL_UNKNOWN                ((uint8_t) (0 << 4))
#define CELL_KNOWN                  ((uint8_t) (1 << 4))

#define PATH_UNIT_LENGTH 10

cell_t gameMap[SIDE_NCELL][SIDE_NCELL];

step_t* findPath(coord_t current, const coord_t target);
void pathFindingReset();
step_t* generatePathCode(coord_t current, coord_t target);
static const coord_t nearest[4] = {
    {-1,  0, E},
    {+1,  0, W},
    { 0, -1, S},
    { 0, +1, N}
};

static const coord_t storage[12] = {
    { GAMEMAP_CENTER - 3, GAMEMAP_CENTER - 1, E},
    { GAMEMAP_CENTER - 3, GAMEMAP_CENTER + 1, E},
    { GAMEMAP_CENTER - 3, GAMEMAP_CENTER    , E},

    { GAMEMAP_CENTER + 3, GAMEMAP_CENTER - 1, W},
    { GAMEMAP_CENTER + 3, GAMEMAP_CENTER    , W},
    { GAMEMAP_CENTER + 3, GAMEMAP_CENTER + 1, W},

    { GAMEMAP_CENTER - 1, GAMEMAP_CENTER - 3, S},
    { GAMEMAP_CENTER    , GAMEMAP_CENTER - 3, S},
    { GAMEMAP_CENTER + 1, GAMEMAP_CENTER - 3, S},

    { GAMEMAP_CENTER - 1, GAMEMAP_CENTER + 3, N},
    { GAMEMAP_CENTER    , GAMEMAP_CENTER + 3, N},
    { GAMEMAP_CENTER + 1, GAMEMAP_CENTER + 3, N}
};

uint8_t getDistance(coord_t A, coord_t B) {
    int8_t delta_x = A.x - B.x;
    int8_t delta_y = A.y - B.y;
    return (uint8_t)(PATH_UNIT_LENGTH * sqrt(delta_x*delta_x+delta_y*delta_y));
}

#define MAX_PATH_SIZE 30
#define GAMEMAP_SIDE_NBR_CELL   9
#define GAMEMAP_CENTER_INDEX    ((GAMEMAP_SIDE_NBR_CELL - 1) / 2)


void mvt_executePath(coord_t *position, const coord_t target, step_t *path) {
    uint8_t i = 0;
    while(path[i] != STOP) {
        switch(path[i]) {
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
        }
        //updatePosition(position, path[i]);
        *position = target;
        i++;
    }
    free(path);
}

void updatePosition(coord_t *position, step_t step) {
    int8_t delta = 0;
    if(step == FORWARD)
        delta = 1;
    else if(step == BACKWARD)
        delta = -1;

    if((step == FORWARD) || (step == BACKWARD)) {
        if(position->t == E)
            position->x += delta;
        else if(position->t == W)
            position->x -= delta;
        else if(position->t == N)
            position->y -= delta;
        else if(position->t == S)
            position->y += delta;
    }

    if(step == LEFT) {
        switch(position->t) {
            case E: position->t = N; break;
            case N: position->t = W; break;
            case W: position->t = S; break;
            case S: position->t = E; break;
        }
    }
    else if(step == RIGHT) {
        switch(position->t) {
            case E: position->t = S; break;
            case N: position->t = E; break;
            case W: position->t = N; break;
            case S: position->t = W; break;
        }
    }
}

void mvt_place(coord_t *position, const coord_t target) {
    coord_t min, tmp;
    uint8_t min_dist = 0xFF;

    // find among all available pieces the cloest one to the target
    for(uint8_t i = 0; i < 12; i++) {
        tmp = storage[i];
        if((gameMap[tmp.x][tmp.y].state & OBSTRUCTION_BITS) == CELL_OCCUPED_RED) {
            uint8_t dist = getDistance((coord_t){.x=tmp.x, .y=tmp.y, .t=tmp.t}, target);
            if(dist <= min_dist) {
                min = tmp;
                min_dist = dist;
            }
        }
    }
    step_t *path = findPath(*position, min);
    mvt_executePath(position, target, path);
    gameMap[min.x][min.y].state &= ~OBSTRUCTION_BITS; // the robot now see the cell free and can go through it
    path = findPath(*position, target);
    mvt_executePath(position, target, path);
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

step_t* findPath(coord_t current, const coord_t target) {
    pathFindingReset(gameMap);

    /*
     * the pahtfinding algorithm start from the target and search the path toward the current position
     * it may seems illogical but this is way, the path obtained through the pointers "parent" starting
     * from the current position is in the right order (current_pos -> intermediate pos -> ... -> target)
     * Otherwise the path's would be reversed (target -> intermiate pos -> ... -> current_pos)
     * this later form needs further process : inverse the list, or process it backward; not handy in any case
     */
    gameMap[current.x][current.y].parent = NULL; // stop code for the generatePathCode's loop
    gameMap[target.x][target.y].state |= CELL_OPEN;
    uint8_t f_min, x_min = 0, y_min = 0;
    
    for(;;) { //use of for(;;) instead of while(true) for compatibility reasons
        /*
        * the default f_min value is 0xFF (0xFF)
        * considering the size of the gameMap a such value should never be obtained
        * TODO: test if uint8_t is enough for the f_score & g_score
        */
       //kendrick amor
        f_min = 0xFF;
        for(uint8_t x = 0; x < SIDE_NCELL; x++)
            for(uint8_t y = 0; y < SIDE_NCELL; y++)
                if(((gameMap[x][y].state & PATH_FIND_BITS) == CELL_OPEN) && (gameMap[x][y].f_score <= f_min)) {
                    f_min = gameMap[x][y].f_score;
                    x_min = x;
                    y_min = y;
                }
        gameMap[x_min][y_min].state = (gameMap[x_min][y_min].state & ~PATH_FIND_BITS) | CELL_CLOSED;
        // The current position has been reached (starting from the target)
        if(((x_min == current.x) && (y_min == current.y)))
            break;

        for(uint8_t w = 0; w < 4; w++) {
            int8_t x = x_min + nearest[w].x;
            int8_t y = y_min + nearest[w].y;

            // Check boundaries
            if((x < 0) || (x >= SIDE_NCELL) || (y < 0) || (y >= SIDE_NCELL)) {}

            // Check for obstacles and already checked cells
            else if(((gameMap[x][y].state & OBSTRUCTION_BITS) == CELL_OCCUPED) ||
                    ((gameMap[x][y].state & OBSTRUCTION_BITS) == CELL_OCCUPED_BLUE)  ||
                    ((gameMap[x][y].state & OBSTRUCTION_BITS) == CELL_OCCUPED_RED)  ||
                    ((gameMap[x][y].state & PATH_FIND_BITS) == CELL_CLOSED)) {}

            // Update scores, parents & state if cell blank or a shorter path to the (x,y) cell was found
            else if((gameMap[x_min][y_min].g_score + PATH_UNIT_LENGTH < gameMap[x][y].g_score) ||
                   ((gameMap[x][y].state & PATH_FIND_BITS) == CELL_BLANK))
            {
                gameMap[x][y].g_score = gameMap[x_min][y_min].g_score + PATH_UNIT_LENGTH;
                gameMap[x][y].f_score = gameMap[x][y].g_score + getDistance((coord_t){.x=x, .y=y}, current);


                gameMap[x][y].parent = &gameMap[x_min][y_min];
                gameMap[x][y].state = (gameMap[x][y].state & ~PATH_FIND_BITS) | CELL_OPEN;
            }
        }
    }
    return generatePathCode(current, target);
}

step_t* generatePathCode(coord_t current, coord_t target) {
    cell_t* icell = &gameMap[current.x][current.y];
    cell_t* icell_old = NULL;
    int8_t delta_x = 0, delta_y = 0;
    angle_t tmp_t = current.t;

    uint16_t path_allocated = MAX_PATH_SIZE;
    uint16_t path_used = 0;
    step_t* path = calloc(path_allocated, sizeof(step_t)); //use of calloc to default initiate to zero (=STOP)

    while(icell->parent != NULL) {
        icell_old = icell;
        icell = icell_old->parent;
        int32_t delta_addr = (icell - icell_old);

        switch(delta_addr) {
            case +SIDE_NCELL:
                delta_x = +1;
                delta_y = 0;
                break;
            case -SIDE_NCELL:
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
        }
        printf("delta_x = %d, delta_y = %d\n", delta_x, delta_y);        

        /*
         * Each iteration in the while loop can add at max 2 steps
         * => the realloc must reserve space for at least 2 step_t
         * to limitate the number of call of realloc, realloc will only
         * happens when the current size is close to the current limit and
         * will reserve 10 more free spaces
         */
        if(path_used + 3 >= path_allocated) {
            path_allocated += 10;
            path = realloc(path, path_allocated*sizeof(step_t));
            for(uint8_t i = path_allocated - 10; i < path_allocated; i++)
                path[i] = STOP;
        }

        switch(tmp_t) {
            case E:
                if(delta_x == 1) {
                    path[path_used++] = FORWARD;
                    path[path_used++] = FORWARD;
                }
                else if(delta_x == -1) {
                    path[path_used++] = BACKWARD;
                    path[path_used++] = BACKWARD;
                }
                else if(delta_y == 1) {
                    path[path_used++] = RIGHT;
                    path[path_used++] = FORWARD;
                    path[path_used++] = FORWARD;
                    tmp_t = S;
                }
                else if(delta_y == -1) {
                    path[path_used++] = LEFT;
                    path[path_used++] = FORWARD;
                    path[path_used++] = FORWARD;
                    tmp_t = N;
                }
                break;
            case N:
                if(delta_x == 1) {
                    path[path_used++] = RIGHT;
                    path[path_used++] = FORWARD;
                    path[path_used++] = FORWARD;
                    tmp_t = E;
                }
                else if(delta_x == -1) {
                    path[path_used++] = LEFT;
                    path[path_used++] = FORWARD;
                    path[path_used++] = FORWARD;
                    tmp_t = W;
                }
                else if(delta_y == 1) {
                    path[path_used++] = BACKWARD;
                    path[path_used++] = BACKWARD;
                }
                else if(delta_y == -1) {
                    path[path_used++] = FORWARD;
                    path[path_used++] = FORWARD;
                }
                break;
            case W:
                if(delta_x == 1) {
                    path[path_used++] = BACKWARD;
                    path[path_used++] = BACKWARD;
                }
                else if(delta_x == -1) {
                    path[path_used++] = FORWARD;
                    path[path_used++] = FORWARD;
                }
                else if(delta_y == 1) {
                    path[path_used++] = LEFT;
                    path[path_used++] = FORWARD;
                    path[path_used++] = FORWARD;
                    tmp_t = S;
                }
                else if(delta_y == -1) {
                    path[path_used++] = RIGHT;
                    path[path_used++] = FORWARD;
                    path[path_used++] = FORWARD;
                    tmp_t = N;
                }
                break;
            case S:
                if(delta_x == 1) {
                    path[path_used++] = LEFT;
                    path[path_used++] = FORWARD;
                    path[path_used++] = FORWARD;
                    tmp_t = E;
                }
                else if(delta_x == -1) {
                    path[path_used++] = RIGHT;
                    path[path_used++] = FORWARD;
                    path[path_used++] = FORWARD;
                    tmp_t = W;
                }
                else if(delta_y == 1) {
                    path[path_used++] = FORWARD;
                    path[path_used++] = FORWARD;
                }
                else if(delta_y == -1) {
                    path[path_used++] = BACKWARD;
                    path[path_used++] = BACKWARD;
                }
                break;
        }
    }

    //might want to use look up table instead, cleaner, hard to figure out
    switch(tmp_t) {
        case E:
            switch(target.t) {
                case E: break;
                case N: path[path_used++] = LEFT; break;
                case W: path[path_used++] = LEFT; path[path_used++] = LEFT; break;
                case S: path[path_used++] = RIGHT; break;
            }
            break;
        case N:
            switch(target.t) {
                case E: path[path_used++] = RIGHT; break;
                case N: break;
                case W: path[path_used++] = LEFT; break;
                case S: path[path_used++] = LEFT; path[path_used++] = LEFT; break;
            }
            break;
        case W:
            switch(target.t) {
                case E: path[path_used++] = LEFT; path[path_used++] = LEFT; break;
                case N: path[path_used++] = RIGHT; break;
                case W: break;
                case S: path[path_used++] = LEFT; break;
            }
            break;
        case S:
            switch(target.t) {
                case E: path[path_used++] = LEFT; break;
                case N: path[path_used++] = LEFT; path[path_used++] = LEFT; break;
                case W: path[path_used++] = RIGHT; break;
                case S: break;
            }
            break;
    }

    return path;
}

void pathFindingReset() {
    for(uint8_t x = 0; x < SIDE_NCELL; x++)
        for(uint8_t y = 0; y < SIDE_NCELL; y++) {
            //set to zero state's bits related to path finding
            gameMap[x][y].state &= ~PATH_FIND_BITS;
            gameMap[x][y].f_score = 0xFF;
            //reset of parent & g_score not really necessary, test before remove
            gameMap[x][y].g_score = 0;
            gameMap[x][y].parent = NULL;
        }
}

void game_init() {
    for(uint8_t x = 0; x < SIDE_NCELL; x++)
        for(uint8_t y = 0; y < SIDE_NCELL; y++)
            gameMap[x][y] = (cell_t){
                .state = 0,
                .f_score = 0xFF, // 0xFF is large enough to be always be greater than the one calculated
                .g_score = 0xFF,
                .parent = NULL
            };
    //for(uint8_t x = GAMEMAP_CENTER-1; x <= GAMEMAP_CENTER+1; x++)
    //    for(uint8_t y = GAMEMAP_CENTER-1; y <= GAMEMAP_CENTER+1; y++)
    //        gameMap[x][y].state = CELL_OCCUPED;

    for(uint8_t i = 0; i < 12; i++) {
        if((i < 3) || ((i > 5) && (i < 9)))
            // Red cells are the one on the left and right sides
            gameMap[storage[i].x][storage[i].y].state |= CELL_OCCUPED_RED;
        else
            // Blue cells are the one on the north and south sides
            gameMap[storage[i].x][storage[i].y].state |= CELL_OCCUPED_BLUE;
    }
}

void updateMap(coord_t *position) {
    coord_t tocheck;
    step_t *path;
    
    for(int8_t i = -1; i <= 1; i++)
        for(int8_t j = -1; j <= 1; j++)
            if((gameMap[i + GAMEMAP_CENTER][j + GAMEMAP_CENTER].state & OBSTRUCTION_BITS) != CELL_OCCUPED) {
                if(i == -1)
                    tocheck = (coord_t){.x=GAMEMAP_CENTER-2, .y=GAMEMAP_CENTER+j, .t=E};
                else if(i == +1)
                    tocheck = (coord_t){.x=GAMEMAP_CENTER+2, .y=GAMEMAP_CENTER+j, .t=W};
                else if(j == -1)
                    tocheck = (coord_t){.x=GAMEMAP_CENTER+i, .y=GAMEMAP_CENTER-2, .t=S};
                else if(j == +1)
                    tocheck = (coord_t){.x=GAMEMAP_CENTER+i, .y=GAMEMAP_CENTER+2, .t=N};
                else if((i == 0) && (j == 0)) {
                    /*
                     * It could looks like there is a case where it's impossible to scan the cell in the middle because it's surrounded on every sides
                     * However, the robot's playing strategy forbidden this kind of situation by always playing the middle case if free
                     * This strategy simplify the scanning and is moreover the winning strategy
                     */
                    for(uint8_t w = 0; w < 4; w++)
                        //if((gameMap[GAMEMAP_CENTER + nearest[w].x][GAMEMAP_CENTER + nearest[w].y].state & OBSTRUCTION_BITS) != CELL_OCCUPED)
                            tocheck = (coord_t){.x=GAMEMAP_CENTER+nearest[w].x, .y=GAMEMAP_CENTER+nearest[w].y, .t=nearest[w].t};
                }
                path = findPath(*position, tocheck);
                mvt_executePath(position, tocheck, path);
                printf("position_x = %d, position_y = %d, position_t = %d\n", position->x, position->y, position->t);
            }
}

int main(void)
{
    cell_t gameMap[SIDE_NCELL][SIDE_NCELL];
    game_init();
    coord_t position = {
        .x = 1,
        .y = SIDE_NCELL-2,
        .t = N,
    };

    printf("position_x = %d, position_y = %d, position_t = %d\n", position.x, position.y, position.t);
    updateMap(&position);
    
    coord_t tocheck = (coord_t){2, 3, E};
    step_t *path;

    path = findPath(position, tocheck);
    mvt_executePath(&position, tocheck, path);
    printf("position_x = %d, position_y = %d, position_t = %d\n", position.x, position.y, position.t);
    
    tocheck = (coord_t){2, 4, E};
    path = findPath(position, tocheck);
    mvt_executePath(&position, tocheck, path);
    printf("position_x = %d, position_y = %d, position_t = %d\n", position.x, position.y, position.t);
    
    return 0;
}