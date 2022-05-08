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
cell_t gameMap[GAMEMAP_SIDE_NCELL][GAMEMAP_SIDE_NCELL];

void init_position() {
    /*
     * The robot must be placed at the center of the map
     * pointing north direction
     */
    chSemObjectInit(&position_s, 1);
    chSemWait(&position_s);
    position_x = GAMEMAP_CENTER;
    position_y = GAMEMAP_CENTER;
    position_t = N;
    chSemSignal(&position_s);
}

void init_path() {
    chSemObjectInit(&path_s, 1);
    reset_path();
}

void reset_path(void) {
    for(uint8_t i = 0; i < MAX_PATH_SIZE; i++)
        path[i] = STOP;
    path_size = 0;
}

void init_gameMap(void) {   
    chSemObjectInit(&gameMap_s, 1);
    chSemWait(&gameMap_s);
    for(uint8_t x = 0; x < GAMEMAP_SIDE_NCELL; x++)
        for(uint8_t y = 0; y < GAMEMAP_SIDE_NCELL; y++)
            gameMap[x][y] = (cell_t){
                .state = 0,
                .f_score = 0xFF,        // 0xFF is large enough to be always be greater than the one calculated 
                .g_score = 0xFF,
                .parent = NULL
            };
    chSemSignal(&gameMap_s);
}