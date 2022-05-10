#include "distance.h"
#include "constants.h"

// semaphore
static BSEMAPHORE_DECL(image_ready_sem, TRUE);
///static semaphore_t distance_sem;
uint16_t d_mm =0;
/*
 *  Returns the case in wich the object facing the Epuck is
 */
distnorm_t scanDistance(void) {
    distnorm_t d;
    //chSemWait(&distance_sem);
    if(d_mm < 1 * CELL_WIDTH)
        d = FIRST_CASE;
    else if((d_mm > 1 * CELL_WIDTH) && (d_mm < 2 * CELL_WIDTH))
        d = SECOND_CASE;
    else if((d_mm > 2 * CELL_WIDTH) && (d_mm < 3 * CELL_WIDTH))
        d = THIRD_CASE;
    else if(d_mm > 3 * CELL_WIDTH)
        d = OUT_MAP;
    //chSemSignal(&distance_sem);
    return d;
}

static THD_WORKING_AREA(waDISTANCE, 128);
static THD_FUNCTION(distance, arg) {
    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    for(;;) {
        //chSemWait(&distance_sem);
        d_mm = VL53L0X_get_dist_mm();
        //chSemSignal(&distance_sem);
        chThdSleepMilliseconds(100);
    }
}

void distance_start(void) {
	chThdCreateStatic(waDISTANCE, sizeof(waDISTANCE), NORMALPRIO,distance, NULL);
}