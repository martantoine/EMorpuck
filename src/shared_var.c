#include "shared_var.h"

const coord_t nearest[4] = {
    {-1,  0, E},
    {+1,  0, W},
    { 0, -1, S},
    { 0, +1, N}
};

const coord_t storage[12] = {
    { GAMEMAP_CENTER - 3, GAMEMAP_CENTER - 1, E},
    { GAMEMAP_CENTER - 3, GAMEMAP_CENTER    , E},
    { GAMEMAP_CENTER - 3, GAMEMAP_CENTER + 1, E},
    
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

    for(uint8_t i = 0; i < 12; i++) {
        if((i < 3) || ((i > 5) && (i < 9)))
            /*
             * Red cells are the one on the left and right sides
             */
            gameMap[storage[i].x][storage[i].y].state |= CELL_OCCUPED_RED;
        else
            /*
             * Blue cells are the one on the north and south sides
             */
            gameMap[storage[i].x][storage[i].y].state |= CELL_OCCUPED_BLUE;
    }
    //chSemSignal(&gameMap_s);
}