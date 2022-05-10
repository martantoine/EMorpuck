#include "main.h"
#include "movements.h"
#include "path.h"
#include "scan.h"
#include "place.h"

#include "msgbus/messagebus.h"
#include "parameter/parameter.h"
#include "spi_comm.h"
#include "camera/dcmi_camera.h"
#include <camera/po8030.h>
#include "i2c_bus.h"

messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);

int main(void)
{
    /** Inits the Inter Process Communication bus. */
    messagebus_init(&bus, &bus_lock, &bus_condvar);

    halInit();
    mpu_init();
    chSysInit();
    spi_comm_start();

    // starts the proximity sensors
    proximity_start();
    ir_button_start();
    calibrate_ir();

    // starts the distance measuring sensor
    distance_start();
    VL53L0X_start();

    // starts the camera
    cam_start();
    dcmi_start();
    po8030_start();
    process_image_start();

    set_stateofgame(STATE_WAITING_FOR_PLAYER);

    while (true)
    {

        chThdSleepMilliseconds(1000);

        test_captors();
    }
    return 0;
}

//fonction de tests des fonctions de mesures de distance et de couleurs
void test_captors(void)
{
    if (scanDistance() == FIRST_CASE)
    {
        set_body_led(LED_ON);
    }
    else
    {
        set_body_led(LED_OFF);
    }

    coord_t position = {
        .x = GAMEMAP_CENTER,
        .y = GAMEMAP_CENTER,
        .t = N,
    };
    step_t* path;
    init_gameMap();

    mvt_init();

    while (true) {
        path = findPath(position, (coord_t){.x=6, .y=6, .t=E});
        mvt_executePath(&position, path);
        path = findPath(position, (coord_t){.x=6, .y=2, .t=N});
        mvt_executePath(&position, path);
        path = findPath(position, (coord_t){.x=2, .y=2, .t=W});
        mvt_executePath(&position, path);
        path = findPath(position, (coord_t){.x=2, .y=6, .t=S});
        mvt_executePath(&position, path);
    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void) {
    chSysHalt("Stack smashing detected");
}