#include "infra.h"

#include <stdio.h>
#include <stdint.h>
#include <ch.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <hal.h>
#include <memory_protection.h>
#include <usbcfg.h>
#include <chprintf.h>

#include "sensors\proximity.h"
#include "leds.h"
#include "shared_var.h"

static THD_WORKING_AREA(waIR_BUTTON, 128);
static THD_FUNCTION(Ir_button, arg) {
    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    for(;;) {
       ir_button_check();
       show_stateofgame();
    }
}

void ir_button_start(void) {
	chThdCreateStatic(waIR_BUTTON, sizeof(waIR_BUTTON), NORMALPRIO,Ir_button, NULL);
}

void ir_button_check(void) {
    uint16_t prox_thr = 100;

    //both sides to begin game
    if((get_calibrated_prox(IR_RIGHT) > prox_thr) && (get_calibrated_prox(IR_LEFT) > prox_thr)) {
        clear_leds();
        set_stateofgame(STATE_PLAYING);
        return;
    }
    // left ir for easy mode
    if(get_calibrated_prox(IR_LEFT) > prox_thr) {
        clear_leds();
        set_stateofgame(STATE_EASY);
        return;
    }

    //right ir for hard mode
    if(get_calibrated_prox(IR_RIGHT) > prox_thr) {
        clear_leds();
        set_stateofgame(STATE_HARD);
        return;
    }
}

void show_stateofgame(void) {
    if(get_stateofgame() == STATE_EASY) {
        set_body_led(LED_OFF);
        set_led(LED7, LED_ON);
    }
    if(get_stateofgame() == STATE_HARD) {
        set_body_led(LED_OFF);
        set_led(LED3,LED_ON);
    }
    if(get_stateofgame() == STATE_PLAYING) {
        set_body_led(LED_OFF);
        set_led(LED1,LED_ON);
    }
    if(get_stateofgame() == STATE_WAITING_FOR_PLAYER) {
        set_body_led(LED_ON);
    }
}