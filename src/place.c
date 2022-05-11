#include "place.h"

#include "movements.h"
#include "shared_var.h"
#include "game.h"
#include "constants.h"
#include "utils.h"
#include "path.h"

void mvt_place(coord_t *position, coord_t *target) {
    coord_t min, tmp;
    uint8_t min_dist = 0xFF;

    // find among all available pieces the cloest one to the target
    for(uint8_t i = 0; i < 12; i++) {
        tmp = storage[i];
        if((gameMap[tmp.x][tmp.y].state & OBSTRUCTION_BITS) == CELL_OCCUPED_RED) {
            uint8_t dist = getDistance((coord_t){.x=tmp.x, .y=tmp.y, .t=tmp.t}, *target);
            if(dist <= min_dist) {
                min = tmp;
                min_dist = dist;
            }
        }
    }
    step_t *path = findPath(*position, min);
    mvt_executePath(position, path);
    gameMap[min.x][min.y].state &= ~OBSTRUCTION_BITS; // the robot now see the cell free and can go through it
    path = findPath(*position, *target);
    mvt_executePath(position, path);
}