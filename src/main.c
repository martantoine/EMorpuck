#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include <usbcfg.h>
#include <main.h>
#include <motors.h>
#include <chprintf.h>


int main(void)
{
    halInit();
    chSysInit();
    mpu_init();
    motors_init();

    right_motor_set_speed(MOTOR_SPEED_LIMIT/2);
    left_motor_set_speed(MOTOR_SPEED_LIMIT/2);
    
    while (1) {
        right_motor_set_speed(MOTOR_SPEED_LIMIT/2);
        left_motor_set_speed(MOTOR_SPEED_LIMIT/2);
        chThdSleepMilliseconds(1000);
        right_motor_set_speed(-MOTOR_SPEED_LIMIT/2);
        left_motor_set_speed(-MOTOR_SPEED_LIMIT/2);
        chThdSleepMilliseconds(1000);        
    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}