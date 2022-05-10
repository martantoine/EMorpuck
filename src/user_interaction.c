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

inline void ingame_check(void);
inline void show_stateofgame(void);

inline void measure_irs(uint8_t *irs);
inline void start_check(uint8_t *irs);

static THD_WORKING_AREA(waIR_BUTTON, 128);
static THD_FUNCTION(Ir_button, arg) {
    chRegSetThreadName(__FUNCTION__);
    (void)arg;
    uint8_t ir_over_threshold[2];
    for(;;) {
        measure_irs(&ir_over_threshold);
        switch(getGameState()) {
            case STATE_START:
                start_check(&ir_over_threshold);
                break;
            case STATE_WAITING_PLAYER:
                stop_check();
                break;
            case STATE_PLAYING:
                stop_check();
                break;
            case STATE_END:
                break;
        }
        show_stateofgame();
        chThdSleepMilliseconds(IR_REFRESH_PERIOD);
    }
}

void ir_button_start(void) {
	chThdCreateStatic(waIR_BUTTON, sizeof(waIR_BUTTON), NORMALPRIO+1, Ir_button, NULL);
}

void measure_irs(uint8_t *irs) {
    if(get_calibrated_prox(IR_1) > IR_THRESHOLD)
        irs[0] = (irs[0] < 0xFF) ? irs[0] + 1 : irs[0];
    else
        irs[0] = 0;
    if(get_calibrated_prox(IR_2) > IR_THRESHOLD)
        irs[1] = (irs[1] < 0xFF) ? irs[1] + 1 : irs[1];
    else
        irs[1] = 0;
}

void start_check(uint8_t *irs) {
    chSemWait(&gamestates_sem);
    if     ((irs[0] > (HOLD_SHORT / IR_REFRESH_PERIOD)) && (irs[1] < (HOLD_SHORT / IR_REFRESH_PERIOD)))
        gamestates = (gamestates & ~DIFFICULTY_BITS) | DIFFICULTY_EASY;
    else if((irs[0] < (HOLD_SHORT / IR_REFRESH_PERIOD)) && (irs[1] > (HOLD_SHORT / IR_REFRESH_PERIOD)))
        gamestates = (gamestates & ~DIFFICULTY_BITS) | DIFFICULTY_HARD;
    else if((irs[0] > (HOLD_SHORT / IR_REFRESH_PERIOD)) && (irs[1] > (HOLD_SHORT / IR_REFRESH_PERIOD)))
        gamestates = (gamestates & ~STATE_BITS) | STATE_WAITING_PLAYER;
    else if((irs[0] > (HOLD_LONG / IR_REFRESH_PERIOD))  && (irs[1] > (HOLD_LONG / IR_REFRESH_PERIOD)))
        gamestates = (gamestates & ~STATE_BITS) | STATE_PLAYING;
    chSemSignal(&gamestates_sem);
}

void ingame_check(void) {
    if((get_calibrated_prox(IR_RIGHT) > IR_THRESHOLD) && (get_calibrated_prox(IR_LEFT) > IR_THRESHOLD))
        gamestates = (gamestates & ~STATE_BITS) | STATE_END;
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