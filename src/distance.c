
/**
 * @file distance.c
 * @author Alexandre Duval(alduval0305@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef DISTANCE_C
#define DISTANCE_C
#include "distance.h"

// semaphore
static BSEMAPHORE_DECL(image_ready_sem, TRUE);
static semaphore_t distance_sem;
uint16_t d_mm =0;
/*
 *  Returns the line's width extracted from the image buffer given
 *  Returns 0 if line not found
 */
distnorm_t scanDistance(void)
{
    distnorm_t d;
    chSemWait(&distance_sem);
    if(d_mm<1000)
    {
        d=FIRST_CASE;
    }
    if((d_mm>1000) & (d_mm<2000))
    {
        d=SECOND_CASE;
    }
    if((d_mm>2000) & (d_mm<3000))
    {
        d=THIRD_CASE;
    }
    if(d_mm>3000)
    {
        d=OUT_MAP;
    }
    chSemSignal(&distance_sem);
    return d;
}

/* 
Working area for the left infra red captor and led
*/

static THD_WORKING_AREA(waDISTANCE, 128);
static THD_FUNCTION(distance, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    while(true)
    {
        chSemWait(&distance_sem);
        d_mm=VL53L0X_get_dist_mm();
        chSemSignal(&distance_sem);
        chThdSleepMilliseconds(100);
      /*  if((VL53L0X_get_dist_mm()>20) && (VL53L0X_get_dist_mm()<200))
        {
            set_led(LED5,LED_ON);
        }
        else
        {
            set_led(LED5, LED_OFF);
        }*/
    }
}
void distance_start(void)
{
	chThdCreateStatic(waDISTANCE, sizeof(waDISTANCE), NORMALPRIO,distance, NULL);
}

#endif /*DISTANCE_C*/