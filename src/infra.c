
/**
 * @file infra.c
 * @author Alexandre Duval(alduval0305@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef INFRA_C
#define INFRA_C
#include "infra.h"


/* 
Working area for the left infra red captor and led
*/

static THD_WORKING_AREA(waIR_BUTTON, 128);
static THD_FUNCTION(Ir_button, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    while(true)
    {
       ir_button_check();
       show_stateofgame();
    }
}

void ir_button_start(void)
{
	chThdCreateStatic(waIR_BUTTON, sizeof(waIR_BUTTON), NORMALPRIO,Ir_button, NULL);
}

void ir_button_check(void)


{       
        uint16_t prox_thr = 100;
        if((get_calibrated_prox(IR_RIGHT)>prox_thr)&& (get_calibrated_prox(IR_LEFT)>prox_thr)) //both sides to begin game
         {
             clear_leds();
             set_stateofgame(STATE_PLAYING);
             return;
         }

        if(get_calibrated_prox(IR_LEFT)>prox_thr)   // left ir for easy mode
        {
            clear_leds();
            set_stateofgame(STATE_EASY);
            return;
        }
        if(get_calibrated_prox(IR_RIGHT)>prox_thr) //right ir for hard mode
         {
             clear_leds();
             set_stateofgame(STATE_HARD);
             return;
         }

}

void show_stateofgame(void)
{
     if(get_stateofgame()==STATE_EASY)
         {
            set_body_led(LED_OFF);
            set_led(LED7, LED_ON);
         }
         if(get_stateofgame()==STATE_HARD)
         {
             set_body_led(LED_OFF);
             set_led(LED3,LED_ON);
         }
         if(get_stateofgame()==STATE_PLAYING)
         {
            set_body_led(LED_OFF);
            set_led(LED1,LED_ON);
            

         }
         if(get_stateofgame()==STATE_WAITING_FOR_PLAYER)
        {
            set_body_led(LED_ON);
        }
}

#endif /*INFRA_C*/