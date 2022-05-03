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
    init_position();

    motors_thd_init();


    findPath(position_x + 2, position_y - 2, N);

    while (1) {
        chSemWait(&path_s);
        chSemWait(&position_s);
        findPath(position_x + 2, position_y - 2, N);
        chSemSignal(&path_s);
        chSemSignal(&position_s);

        chThdYield();
        
        chSemWait(&path_s);
        chSemWait(&position_s);
        findPath(position_x - 2, position_y + 2, S);
        chSemSignal(&path_s);
        chSemSignal(&position_s);

        chThdYield();
        //chThdSleepMilliseconds(40);
    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}