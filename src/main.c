#include "main.h"
#include "motion/movements.h"
#include "motion/path.h"
#include "sensors/sensor_color.h"
#include "sensors/sensor_distance.h"
#include "sensors/sensor_ir.h"
#include "game.h"

#include <stdlib.h>
#include <time.h>

messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);

inline void gameMap_init(cell_t **gameMap) {
    for(uint8_t x = 0; x < GAMEMAP_SIDE_NCELL; x++)
        for(uint8_t y = 0; y < GAMEMAP_SIDE_NCELL; y++)
            gameMap[x][y] = (cell_t){
                .state = 0,
                .f_score = 0xFF, // 0xFF is large enough to be always be greater than the one calculated
                .g_score = 0xFF,
                .parent = NULL
            };

    for(uint8_t i = 0; i < 12; i++) {
        if((i < 3) || ((i > 5) && (i < 9)))
            // Red cells are the one on the left and right sides
            gameMap[storage[i].x][storage[i].y].state |= CELL_OCCUPED_RED;
        else
            // Blue cells are the one on the north and south sides
            gameMap[storage[i].x][storage[i].y].state |= CELL_OCCUPED_BLUE;
    }
}

int main(void)
{
    /** Inits the Inter Process Communication bus. */
    messagebus_init(&bus, &bus_lock, &bus_condvar);

    // MCU, ChibiOS init
    halInit();
    mpu_init();
    chSysInit();
    spi_comm_start();
    srand(time(0));

    // Sensors
    user_interaction_init();
    tof_init();
    color_init();

    coord_t position = {
        .x = GAMEMAP_CENTER,
        .y = GAMEMAP_CENTER,
        .t = N,
    };
    step_t* path;
    /*
     * the cell's position is stored implincitly in the address :
     * the cell of position (i,j) is accesssed with gameMap[i,j]
     */
    cell_t gameMap[GAMEMAP_SIDE_NCELL][GAMEMAP_SIDE_NCELL];
    gameMap_init(gameMap);

    mvt_init();

    while (true)
    {
        if((gamestates & STATE_BITS) == STATE_WAITING_PLAYER)
            chThdSleepMilliseconds(500);
        else if((gamestates & STATE_BITS) == STATE_PLAYING) {
            updateMap(gameMap, &position);
            coord_t target;
            if((gamestates & DIFFICULTY_BITS) == DIFFICULTY_EASY)
                target = place_easy(gameMap);
            else if((gamestates & DIFFICULTY_BITS) == DIFFICULTY_EASY)
                target = place_hard(gameMap);
            mvt_place(gameMap, &position, &target);
        }
        //chThdSleepMilliseconds(1000);
        //test_captors();
    }
    return 0;
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void) {
    chSysHalt("Stack smashing detected");
}