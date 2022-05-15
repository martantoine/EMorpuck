#include "scan.h"

#include "defs.h"
#include "motion/path.h"
#include "motion/movements.h"
#include "sensors/sensor_color.h"
#include "sensors/sensor_distance.h"
#include "game.h"

void updateMap(cell_t gameMap[SIDE_NCELL][SIDE_NCELL], coord_t *position) {
    coord_t tocheck;
    step_t *path;

    for(int8_t i = -1 + GAMEMAP_CENTER; i <= 1 + GAMEMAP_CENTER; i++)
        for(int8_t j = -1 + GAMEMAP_CENTER; j <= 1 + GAMEMAP_CENTER; j++) {
            if((gameMap[i][j].state & OBSTRUCTION_BITS) == (CELL_OCCUPED_RED || CELL_OCCUPED_BLUE))
                gameMap[i][j].state = (gameMap[i][j].state & ~KNOWLEDGE_BITS) | CELL_KNOWN;
            else
                gameMap[i][j].state = (gameMap[i][j].state & ~KNOWLEDGE_BITS) | CELL_UNKNOWN;
        }

    for(int8_t i = 0; i < 9; i++) {
        if((gameMap[scanning_order[i].x][scanning_order[i].y].state & OBSTRUCTION_BITS) != (CELL_OCCUPED_RED || CELL_OCCUPED_BLUE)) {
            tocheck = scanning_order[i];
            if((scanning_order[i].x == GAMEMAP_CENTER) && (scanning_order[i].y == GAMEMAP_CENTER)) {
                /*
                 * It could looks like there is a case where it's impossible to scan the cell in the middle because it's surrounded on every sides
                 * However, the robot's playing strategy forbidden this kind of situation by always playing the middle case if free
                 * This strategy simplify the scanning and is moreover the winning strategy
                 */
                for(uint8_t w = 0; w < 4; w++)
                    if((gameMap[GAMEMAP_CENTER + nearest[w].x][GAMEMAP_CENTER + nearest[w].y].state & OBSTRUCTION_BITS) != (CELL_OCCUPED_RED || CELL_OCCUPED_BLUE))
                        tocheck = (coord_t){.x=GAMEMAP_CENTER+nearest[w].x, .y=GAMEMAP_CENTER+nearest[w].y, .t=nearest[w].t};
            }
            
            path = findPath(gameMap, *position, tocheck);
            mvt_executePath(position, tocheck, path);
            uint8_t c_result = sensor_measure_color();
            uint8_t d_result = sensor_distance_norm();
            chSemWait(&gamestates_sem);
            if((c_result == RED) && (d_result != OUT_MAP))
                gamestates = (gamestates & ~SCANNING_BITS) | SCANNING_RED;
            else if((c_result == BLUE) && (d_result != OUT_MAP))
                gamestates = (gamestates & ~SCANNING_BITS) | SCANNING_BLUE;
            else if((c_result == NONE) || (d_result == OUT_MAP))
                gamestates = (gamestates & ~SCANNING_BITS) | SCANNING_NONE;
            chSemSignal(&gamestates_sem);
            chThdSleepMilliseconds(1000); // to make leds' color visible
            chSemWait(&gamestates_sem);
            gamestates = (gamestates & ~SCANNING_BITS) | SCANNING_NOT_SCANNING;
            chSemSignal(&gamestates_sem);

            if((d_result != OUT_MAP) && (c_result == BLUE))
                switch(position->t) {
                    case E:
                        gameMap[position->x+d_result][position->y].state |= CELL_OCCUPED_BLUE;
                        gameMap[position->x+d_result][position->y].state &= ~KNOWLEDGE_BITS;
                        break;
                    case W:
                        gameMap[position->x-d_result][position->y].state |= CELL_OCCUPED_BLUE;
                        gameMap[position->x-d_result][position->y].state &= ~KNOWLEDGE_BITS;
                        break;
                    case N:
                        gameMap[position->x][position->y-d_result].state |= CELL_OCCUPED_BLUE;
                        gameMap[position->x][position->y-d_result].state &= ~KNOWLEDGE_BITS;
                        break;
                    case S:
                        gameMap[position->x][position->y+d_result].state |= CELL_OCCUPED_BLUE;
                        gameMap[position->x][position->y+d_result].state &= ~KNOWLEDGE_BITS;
                        break;
                }
        }
    }
}