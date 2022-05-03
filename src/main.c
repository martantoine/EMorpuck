#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <ch.h>
#include <hal.h>
#include <memory_protection.h>
#include <usbcfg.h>
#include <chprintf.h>

#include "main.h"
#include "motors_thd.h"
#include "shared_var.h"
#include "movement.h"
#include "motors_driver.h"
#include "path.h"

int main(void)
{
    halInit();
    chSysInit();
    mpu_init();
    init_path();
    init_gameMap();
    init_position();
    
    motors_thd_init();
    /*
    mvt_forward();
    chThdSleepMilliseconds(1000);
    mvt_backward();
    chThdSleepMilliseconds(1000);
    mvt_left();
    chThdSleepMilliseconds(1000);
    mvt_right();
    chThdSleepMilliseconds(1000);
    */
    while (true) {
        findPath(7, 7, E);
        readpath();
        findPath(7, 1, E);
        readpath();
        findPath(4, 4, N);
        readpath();
        
    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}