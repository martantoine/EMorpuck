#include "scan.h"

#include "shared_var.h"
#include "constants.h"
#include "path.h"

void updateMap(void) {
    for(int8_t i = -1; i <= 1; i++)
        for(int8_t j = -1; j <= 1; j++)
            if((gameMap[i + GAMEMAP_CENTER][j + GAMEMAP_CENTER].state & OBSTRUCTION_BITS) == CELL_FREE) {
                angle_t target_angle;
                if(i == -1)
                    target_angle = E;
                else if(i == +1)
                    target_angle = W;
                else if(j == -1)
                    target_angle = S;
                else if(j == +1)
                    target_angle = N;
                else if((i == 0) && (j == 0)) {
                    /*
                     * More complex case where the cell to check is at the center and some sides might be obstructed
                     * The case where all sides are occuped is ignored
                     */
                    if((gameMap[GAMEMAP_CENTER - 1][GAMEMAP_CENTER].state & OBSTRUCTION_BITS) == CELL_FREE)
                        target_angle = E;
                    else if((gameMap[GAMEMAP_CENTER + 1][GAMEMAP_CENTER].state & OBSTRUCTION_BITS) == CELL_FREE)
                        target_angle = W;
                    else if((gameMap[GAMEMAP_CENTER][GAMEMAP_CENTER - 1].state & OBSTRUCTION_BITS) == CELL_FREE)
                        target_angle = S;
                    else if((gameMap[GAMEMAP_CENTER][GAMEMAP_CENTER + 1].state & OBSTRUCTION_BITS) == CELL_FREE)
                        target_angle = N;
                }
                findPath(i + GAMEMAP_CENTER, j + GAMEMAP_CENTER, target_angle);
                chThdYield();
                uint8_t result = scanLine();
                gameMap[i + GAMEMAP_CENTER][j + GAMEMAP_CENTER].state += result;
            }
}

uint8_t scanLine(void)
{
    /*
     * must read the camera shared memory and TOF shared memory
     */
    //chThdWait(camera_s);
    //chThdWait(tof_s);
    
    //chThdSignal(camera_s);
    //chThdSignal(tof_s);
    return CELL_OCCUPED_BLUE;
}