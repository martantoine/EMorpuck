#include "scan.h"

#include "shared_var.h"
#include "constants.h"
#include "path.h"
#include "movements.h"

void updateMap(coord_t *position) {
    coord_t tocheck;
    step_t *path;

    for(int8_t i = -1; i <= 1; i++)
        for(int8_t j = -1; j <= 1; j++)
            if((gameMap[i + GAMEMAP_CENTER][j + GAMEMAP_CENTER].state & OBSTRUCTION_BITS) == CELL_FREE) {
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
                        if((gameMap[GAMEMAP_CENTER + nearest[w].x][GAMEMAP_CENTER + nearest[w].y].state & OBSTRUCTION_BITS) == CELL_FREE)
                            tocheck = (coord_t){.x=GAMEMAP_CENTER+nearest[w].x, .y=GAMEMAP_CENTER+nearest[w].y, .t=nearest[w].t};
                }
                path = findPath(*position, tocheck);
                mvt_executePath(position, path);

                //uint8_t result = scanLine();
                //gameMap[i + GAMEMAP_CENTER][j + GAMEMAP_CENTER].state += result;
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