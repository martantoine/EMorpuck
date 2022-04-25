#include "shared_var.h"

semaphore_t position_s;
int8_t position_x;
int8_t position_y;
angle_t position_t;

semaphore_t path_s;
step_t path[MAX_PATH_STEPS];
//memory_heap_t* path;
/* ideally path should be dynamicaly allocated
 * however, the c standard library allocator doesn't work with multi threading
 * the heap allocator of ChibiOS must be used instead
 * need to figure out how thoo
 */

void init_position()
{
    chSemObjectInit(&position_s, 1);
    position_x = 0;
    position_y = 0;
    position_t = 0;
}

void init_path()
{
    //chHeapObjectInit(path, step_t)
    chSemObjectInit(&path_s, 2);
    for(int i=0; i < MAX_PATH_STEPS; i++)
        path[i] = STOP;
    //path = NULL;
}