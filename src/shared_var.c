#include "shared_var.h"

semaphore_t gameMap_s;
cell_t gameMap[GAMEMAP_SIDE_NCELL][GAMEMAP_SIDE_NCELL];

void init_gameMap(void) {   
    //chSemObjectInit(&gameMap_s, 1);
    //chSemWait(&gameMap_s);
    for(uint8_t x = 0; x < GAMEMAP_SIDE_NCELL; x++)
        for(uint8_t y = 0; y < GAMEMAP_SIDE_NCELL; y++)
            gameMap[x][y] = (cell_t){
                .state = 0,
                .f_score = 0xFF,        // 0xFF is large enough to be always be greater than the one calculated 
                .g_score = 0xFF,
                .parent = NULL
            };
    //chSemSignal(&gameMap_s);
}