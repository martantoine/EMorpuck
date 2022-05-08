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
#include "shared_var.h"
#include "movements.h"
#include "path.h"

int main(void) {
    halInit();
    chSysInit();
    mpu_init();

    coord_t position = {
        .x = GAMEMAP_CENTER,
        .y = GAMEMAP_CENTER,
        .t = N,
    };
    step_t* path;
    init_gameMap();
    
    mvt_init();

    while (true) {
        path = findPath(position, (coord_t){.x=7, .y=7, .t=N});
        mvt_executePath(position, path);
        path = findPath(position, (coord_t){.x=7, .y=1, .t=N});
        mvt_executePath(position, path);
        path = findPath(position, (coord_t){.x=1, .y=1, .t=N});
        mvt_executePath(position, path);
        path = findPath(position, (coord_t){.x=1, .y=7, .t=N});
        mvt_executePath(position, path);
    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}