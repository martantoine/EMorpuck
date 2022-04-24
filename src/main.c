#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include "motors_thd.h"
#include "main.h"
#include "shared_var.h"
#include <usbcfg.h>
#include <chprintf.h>

int main(void)
{
    halInit();
    chSysInit();
    mpu_init();
    motors_init();
    motors_thd_init();

    while (1) {
        chSemWait(&path_s);
        path = malloc(10*sizeof(step_t));
        path[0] = FORWARD;
        path[1] = FORWARD;
        path[2] = FORWARD;
        path[3] = LEFT;
        path[4] = FORWARD;
        path[5] = FORWARD;
        path[6] = FORWARD;
        path[7] = FORWARD;
        path[8] = RIGHT;
        path[9] = RIGHT;
        chSemSignal(&path_s);
        chThdSleepMilliseconds(4000);        
    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}