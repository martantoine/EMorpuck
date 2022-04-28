#include "shared_var.h"

semaphore_t position_s;
int8_t position_x;
int8_t position_y;
angle_t position_t;

semaphore_t path_s;
memory_heap_t* path_h;
step_t* path;
uint32_t path_size;
uint32_t path_alloc_size;
/* ideally path should be dynamicaly allocated
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
    position_x = 0;
    position_y = 0;
    position_t = 0;
    chSemSignal(&position_s);
}

void init_path()
{
    chSemObjectInit(&path_s, 1);
    chSemWait(&path_s);
    path_alloc_size = DEFAULT_NBR_STEPS_IN_PATH;
    chHeapObjectInit(path_h, path, path_alloc_size * sizeof(step_t));
    for(int i=0; i < DEFAULT_NBR_STEPS_IN_PATH; i++)
        path[i] = STOP;
    chSemSignal(&path_s);
}

void init_gameMap(void)
{   
    chSemObjectInit(&gameMap_s, 1);
    chSemWait(&gameMap_s);
    for(uint8_t i = 0; i < GAMEMAP_SIDE_NBR_CELL; i++)
        for(uint8_t j = 0; j < GAMEMAP_SIDE_NBR_CELL; j++)
            gameMap[i][j] = (cell_t){
                .state = CELL_FREE,
                .f_score = 0xFF,        // 0xFF is large enough to be always be greater than the one calculated 
                .parent = NULL
            };
    chSemSignal(&gameMap_s);
}