#include "shared_var.h"

semaphore_t position_s;
int8_t position_x;
int8_t position_y;
angle_t position_t;

semaphore_t path_s;
step_t path[MAX_PATH_SIZE];
uint8_t path_size;
/* 
 * ideally path should be dynamicaly allocated
 * however, the c standard library allocator doesn't work with multi threading
 * the heap allocator of ChibiOS must be used instead
 * need to figure out how thoo
 */

semaphore_t gameMap_s;
cell_t gameMap[GAMEMAP_SIDE_NBR_CELL][GAMEMAP_SIDE_NBR_CELL];

void init_position()
{
    chSemObjectInit(&position_s, 1);
    chSemWait(&position_s);
    position_x = GAMEMAP_CENTER_INDEX;
    position_y = GAMEMAP_CENTER_INDEX;
    position_t = N;
    chSemSignal(&position_s);
}

void init_path()
{
    chSemObjectInit(&path_s, 1);
    chSemWait(&path_s);
    reset_path();
    chSemSignal(&path_s);
}

void reset_path(void)
{
    for(uint8_t i = 0; i < MAX_PATH_SIZE; i++)
        path[i] = STOP;
    path_size = 0;
}

void init_gameMap(void)
{   
    chSemObjectInit(&gameMap_s, 1);
    chSemWait(&gameMap_s);
    for(uint8_t i = 0; i < GAMEMAP_SIDE_NBR_CELL; i++)
        for(uint8_t j = 0; j < GAMEMAP_SIDE_NBR_CELL; j++)
            gameMap[i][j] = (cell_t){
                .state = 0,
                .f_score = 0xFF,        // 0xFF is large enough to be always be greater than the one calculated 
                .parent = NULL
            };
    chSemSignal(&gameMap_s);
}