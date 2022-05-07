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

    while (true) {
        findPath(7, 7, N);
        readpath();
        findPath(7, 1, N);
        readpath();
        findPath(1, 1, N);
        readpath();
        findPath(1, 7, N);
        readpath();
        
    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}