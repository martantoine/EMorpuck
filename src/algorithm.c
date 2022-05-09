#include "algorithm.h"
#include <stdlib.h>
#include <time.h>

#include "constants.h"
#include "typedef.h"

/*this function should be used after verifying that table as some free space, if not the case endgame()*/
void place_easy(uint8_t **table) {
    for(;;) {
        srand(time(0)); //init should be done only once (separate function called by main)
        //assign 2 random coordinates between 0 and 3
        //use of coord_t instead of 2 int because this struct takes 1 bytes instead of 2 bytes
        coord_t tmp = {
            .x = GAMEMAP_CENTER - 3 + (rand() % 4),
            .y = GAMEMAP_CENTER - 3 + (rand() % 4)
        };

        if(table[tmp.x][tmp.y] == CELL_FREE) {
            table[tmp.x][tmp.y] = CELL_OCCUPED_RED;
            break;
        }
    }
}