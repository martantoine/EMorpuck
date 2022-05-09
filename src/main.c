#include "main.h"
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

    if (scanColor() == RED)

    {
        clear_leds();
        set_rgb_led(0, 10, 0, 0);
        set_rgb_led(1, 10, 0, 0);
        set_rgb_led(2, 10, 0, 0);
        set_rgb_led(3, 10, 0, 0);
    }
    if (scanColor() == BLUE)
    {
        clear_leds();
        set_rgb_led(0, 0, 0, 10);
        set_rgb_led(1, 0, 0, 10);
        set_rgb_led(2, 0, 0, 10);
        set_rgb_led(3, 0, 0, 10);
    }
}



#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}