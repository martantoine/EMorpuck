#include "main.h"
#include "motion/movements.h"
#include "sensors/sensor_color.h"
#include "sensors/sensor_distance.h"
#include "sensors/sensor_ir.h"
#include "game.h"
#include "scan.h"
#include "memory_protection.h"

#include <ch.h>
#include <hal.h>
#include <spi_comm.h>
#include <stdlib.h>
#include <time.h>
#include <leds.h>

#include <camera/po8030.h>
#include <spi_comm.h>

messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);

int main(void) {
    // MCU, ChibiOS init
    halInit();
    mpu_init();
    chSysInit();
    spi_comm_start();
    // Inits the Inter Process Communication bus
    messagebus_init(&bus, &bus_lock, &bus_condvar);
    clear_leds();
    //srand(time(0));
    // Sensors
    cell_t gameMap[SIDE_NCELL][SIDE_NCELL];
    game_init(gameMap);
    sensor_color_init();
    sensor_ir_init();
    sensor_distance_init();
    // Actuators
    mvt_init();

    /*
     * the cell's position is stored implincitly in the address :
     * the cell of position (i,j) is accesssed with gameMap[i,j]
     */
    coord_t position = {
        .x = 1,
        .y = SIDE_NCELL-2,
        .t = N,
    };

    //
    chThdSetPriority(NORMALPRIO);
    while(1) { 
        //gamestates = WINNER_RED | STATE_END;
        if((gamestates & STATE_BITS) == STATE_WAITING_PLAYER) {}
        else if((gamestates & STATE_BITS) == STATE_PLAYING) {
            updateMap(gameMap, &position);
            coord_t target;
            if((gamestates & DIFFICULTY_BITS) == DIFFICULTY_EASY)
                target = place_easy(gameMap);
            else if((gamestates & DIFFICULTY_BITS) == DIFFICULTY_EASY)
                target = place_hard(gameMap);
            mvt_place(gameMap, &position, target);
            gamestates = (gamestates & ~STATE_BITS) | STATE_WAITING_PLAYER;
        }
        else if(((gamestates & STATE_BITS) == STATE_START) || ((gamestates & STATE_BITS) == STATE_END)) {}
        check_end_game(gameMap);
        show_stateofgame();
        chThdSleepMilliseconds(200);
    }
    return 0;
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void) {
    chSysHalt("Stack smashing detected");
}