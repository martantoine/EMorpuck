
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


/* 
Working area for the left infra red captor and led
*/

static THD_WORKING_AREA(waDISTANCE, 128);
static THD_FUNCTION(distance, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    while(true)
    {
        if((VL53L0X_get_dist_mm()>20) && (VL53L0X_get_dist_mm()<200))
        {
            set_led(LED5,LED_ON);
        }
        else
        {
            set_led(LED5, LED_OFF);
        }
    }
}
void distance_start(void)
{
	chThdCreateStatic(waDISTANCE, sizeof(waDISTANCE), NORMALPRIO,distance, NULL);
}

#endif /*DISTANCE_C*/