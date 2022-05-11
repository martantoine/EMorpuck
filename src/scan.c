#include "scan.h"

#include "defs.h"
#include "motion/path.h"
#include "motion/movements.h"

void updateMap(cell_t gameMap[SIDE_NCELL][SIDE_NCELL], coord_t *position) {
    coord_t tocheck;
    step_t *path;

    for(int8_t i = -1; i <= 1; i++)
        for(int8_t j = -1; j <= 1; j++)
            if((gameMap[i + SIDE_NCELL][j + SIDE_NCELL].state & OBSTRUCTION_BITS) == CELL_FREE) {
                if(i == -1)
                    tocheck = (coord_t){.x=SIDE_NCELL-2, .y=SIDE_NCELL+j, .t=E};
                else if(i == +1)
                    tocheck = (coord_t){.x=SIDE_NCELL+2, .y=SIDE_NCELL+j, .t=W};
                else if(j == -1)
                    tocheck = (coord_t){.x=SIDE_NCELL+i, .y=SIDE_NCELL-2, .t=S};
                else if(j == +1)
                    tocheck = (coord_t){.x=SIDE_NCELL+i, .y=SIDE_NCELL+2, .t=N};
                else if((i == 0) && (j == 0)) {
                    /*
                     * It could looks like there is a case where it's impossible to scan the cell in the middle because it's surrounded on every sides
                     * However, the robot's playing strategy forbidden this kind of situation by always playing the middle case if free
                     * This strategy simplify the scanning and is moreover the winning strategy
                     */
                    for(uint8_t w = 0; w < 4; w++)
                        if((gameMap[SIDE_NCELL + nearest[w].x][SIDE_NCELL + nearest[w].y].state & OBSTRUCTION_BITS) == CELL_FREE)
                            tocheck = (coord_t){.x=SIDE_NCELL+nearest[w].x, .y=SIDE_NCELL+nearest[w].y, .t=nearest[w].t};
                }
                path = findPath(gameMap, *position, tocheck);
                mvt_executePath(position, path);

                //uint8_t result = scanLine();
                //gameMap[i + SIDE_NCELL][j + SIDE_NCELL].state += result;
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