#include "user_interaction.h""

#include <stdio.h>
#include <stdint.h>
#include <ch.h>
#include <time.h>
#include <math.h>
#include <memory_protection.h>
//#include <usbcfg.h>
#include <chprintf.h>

#include "sensors\proximity.h"
#include "leds.h"
#include "shared_var.h"
#include "game.h"

inline void start_check(void);
inline void stop_check(void);
inline void show_stateofgame(void);

static THD_WORKING_AREA(waIR_BUTTON, 128);
static THD_FUNCTION(Ir_button, arg) {
    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    for(;;) {
        switch(getGameState()) {
            case GAMESTATE_START:
                start_check();
                break;
            case GAMESTATE_WAITING_PLAYER:
                stop_check();
                break;
            case GAMESTATE_PLAYING:
                stop_check();
                break;
            case GAMESTATE_END:
                break;
        }
        show_stateofgame();
    }
}

void ir_button_start(void) {
	chThdCreateStatic(waIR_BUTTON, sizeof(waIR_BUTTON), NORMALPRIO,Ir_button, NULL);
}

void start_check(void) {
    //both sides to begin game
    if((get_calibrated_prox(IR_RIGHT) > IR_THRESHOLD) && (get_calibrated_prox(IR_LEFT) > IR_THRESHOLD))
        set_stateofgame(GAMESTATE_PLAYING);

    // left ir for easy mode
    else if((get_calibrated_prox(IR_RIGHT) > IR_THRESHOLD) && (get_calibrated_prox(IR_LEFT) < IR_THRESHOLD))
        set_stateofgame(DIFFICULTY_EASY);

    //right ir for hard mode
    else if((get_calibrated_prox(IR_RIGHT) < IR_THRESHOLD) && (get_calibrated_prox(IR_LEFT) > IR_THRESHOLD))
        set_stateofgame(DIFFICULTY_HARD);
}

void stop_check(void) {
    //both sides to begin game
    if((get_calibrated_prox(IR_RIGHT) > IR_THRESHOLD) && (get_calibrated_prox(IR_LEFT) > IR_THRESHOLD))
        set_stateofgame(GAMESTATE_END);
}

void show_stateofgame(void) {
   swicht(get_difficulty()) {
    case DIFFICULTY_EASY:
        set_body_led(LED_OFF);
        set_led(LED7, LED_ON);
        break;
    case DIFFICULTY_HARD:
        set_body_led(LED_OFF);
        set_led(LED3, LED_ON);
        break;
    }

    if(get_state() == STATE_PLAYING) {
        set_body_led(LED_OFF);
        set_led(LED1, LED_ON);
    }
    if(get_state() == STATE_WAITING_FOR_PLAYER) {
        set_body_led(LED_ON);
    }
}