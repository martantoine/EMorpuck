#include "tof.h"
#include "constants.h"
#include <sensors/VL53L0X/VL53L0X.h>
#include <ch.h>

static semaphore_t tof_sem;
uint16_t tof_mm = 0;

/*
 *  Returns the case in wich the object facing the Epuck is
 */
distnorm_t tof_measure(void) {
    distnorm_t dist;
    chSemWait(&tof_sem);
    if     ((tof_mm < 1 * CELL_WIDTH))
        dist = FIRST_CASE;
    else if((tof_mm > 1 * CELL_WIDTH) && (tof_mm < 2 * CELL_WIDTH))
        dist = SECOND_CASE;
    else if((tof_mm > 2 * CELL_WIDTH) && (tof_mm < 3 * CELL_WIDTH))
        dist = THIRD_CASE;
    else if((tof_mm > 3 * CELL_WIDTH))
        dist = OUT_MAP;
    chSemSignal(&tof_sem);
    return dist;
}

static THD_WORKING_AREA(waTOF, 128);
static THD_FUNCTION(TOF, arg) {
    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    for(;;) {
        chSemWait(&tof_sem);
        tof_mm = VL53L0X_get_dist_mm();
        chSemSignal(&tof_sem);
        chThdSleepMilliseconds(TOF_REFRESH_T);
    }
}

void tof_init(void) {
    VL53L0X_start();
	chThdCreateStatic(waTOF, sizeof(waTOF), TOF_PRIORITY, TOF, NULL);
}