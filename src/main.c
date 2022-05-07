#include "main.h"
#include "msgbus/messagebus.h"
#include "parameter/parameter.h"
#include "camera/dcmi_camera.h"
#include <camera/po8030.h>

// MUTEX_DECL(bus_lock);
// CONDVAR_DECL(bus_condvar);

int main(void)
{
    /** Inits the Inter Process Communication bus. */
  //  messagebus_init(&bus, &bus_lock, &bus_condvar);
    halInit();
    mpu_init();  
    chSysInit();

 //   spi_comm_start();
  //  proximity_start();
   // ir_button_start();
   
   
  //  calibrate_ir();
 //   clear_leds();
  //  distance_start();
   // VL53L0X_start();
    //set_stateofgame(STATE_WAITING_FOR_PLAYER);

    //starts the camera
    dcmi_start();
	po8030_start();

    process_image_start();

    while(true)
    {
        chThdSleepMilliseconds(1000);
    }
    return 0;
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}