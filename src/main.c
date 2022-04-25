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

int main(void)
{
    halInit();
    chSysInit();
    mpu_init();
    init_path();
    init_position();

    motors_thd_init();

    while (1) {
        chSemWait(&path_s);
        //chHeapAlloc(path, 13 * sizeof(step_t));
        //path = (step_t*) malloc(13*sizeof(step_t));
        *(path + 0 * sizeof(step_t)) = FORWARD;
        *(path + 1 * sizeof(step_t)) = FORWARD;
        *(path + 2 * sizeof(step_t)) = FORWARD;
        *(path + 3 * sizeof(step_t)) = LEFT;
        *(path + 4 * sizeof(step_t)) = BACKWARD;
        *(path + 5 * sizeof(step_t)) = BACKWARD;
        *(path + 6 * sizeof(step_t)) = FORWARD;
        *(path + 7 * sizeof(step_t)) = FORWARD;
        *(path + 8 * sizeof(step_t)) = RIGHT;
        *(path + 9 * sizeof(step_t)) = BACKWARD;
        *(path + 10 * sizeof(step_t)) = BACKWARD;
        *(path + 11 * sizeof(step_t)) = BACKWARD;
        *(path + 12 * sizeof(step_t)) = STOP;
        chSemSignal(&path_s);
        //chThdResume(motor_get_trp(), 0);
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