#include "main.h"
#include "movements.h"
#include "path.h"
#include "scan.h"
#include "place.h"
#include "constants.h"
#include "user_interaction.h"
#include "tof.h"
#include "color_scan.h"
#include "game.h"

#include "msgbus/messagebus.h"
#include "parameter/parameter.h"
#include "spi_comm.h"
#include "camera/dcmi_camera.h"
#include <camera/po8030.h>
#include "i2c_bus.h"
#include <chprintf.h>
#include <usbcfg.h>
#include <stdlib.h>
#include <time.h>

messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);

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
    gameMap_init();

    mvt_init();

    while (true)
    {
        if((gamestates & STATE_BITS) == STATE_WAITING_PLAYER)
            chThdSleepMilliseconds(500);
        else if((gamestates & STATE_BITS) == STATE_PLAYING) {
            updateMap(&position);
            coord_t target;
            if((gamestates & DIFFICULTY_BITS) == DIFFICULTY_EASY)
                target = place_easy(gameMap);
            else if((gamestates & DIFFICULTY_BITS) == DIFFICULTY_EASY)
                target = place_hard(gameMap);
            mvt_place(&position, &target);
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