#include "user_interaction.h"
#include "sensors\proximity.h"
#include "leds.h"
#include "shared_var.h"
#include "game.h"

#include <ch.h>
#include <memory_protection.h>
//#include <usbcfg.h>

inline void measure_irs(uint8_t *irs);
inline void start_check(uint8_t *irs);
inline void ingame_check(uint8_t *irs);
inline void show_stateofgame(void);

static THD_WORKING_AREA(waUserInteraction, 128);
static THD_FUNCTION(UserInteraction, arg) {
    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    uint8_t ir_over_threshold[2];
    for(;;) {
        measure_irs(ir_over_threshold);
        chSemWait(&gamestates_sem);
        uint8_t gamestates_tmp = gamestates;
        chSemSignal(&gamestates_sem);
        if((gamestates_tmp & STATE_BITS) == STATE_START)
                start_check(ir_over_threshold);
        else if((gamestates_tmp & STATE_BITS) == STATE_WAITING_PLAYER)
                ingame_check(ir_over_threshold);
        else if((gamestates_tmp & STATE_BITS) == STATE_PLAYING)
                ingame_check(ir_over_threshold);
        show_stateofgame();
        chThdSleepMilliseconds(USERINTERACTION_REFRESH_T);
    }
}

void user_interaction_init(void) {
    proximity_start();
	chThdCreateStatic(UserInteraction, sizeof(waUserInteraction), IR_THD_PRIOTIRY, UserInteraction, NULL);
    calibrate_ir();
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
    if     ((irs[0] > (HOLD_SHORT / USERINTERACTION_REFRESH_T)) && (irs[1] < (HOLD_SHORT / USERINTERACTION_REFRESH_T)))
        gamestates = (gamestates & ~DIFFICULTY_BITS) | DIFFICULTY_EASY;
    else if((irs[0] < (HOLD_SHORT / USERINTERACTION_REFRESH_T)) && (irs[1] > (HOLD_SHORT / USERINTERACTION_REFRESH_T)))
        gamestates = (gamestates & ~DIFFICULTY_BITS) | DIFFICULTY_HARD;
    else if((irs[0] > (HOLD_SHORT / USERINTERACTION_REFRESH_T)) && (irs[1] > (HOLD_SHORT / USERINTERACTION_REFRESH_T)))
        gamestates = (gamestates & ~STATE_BITS) | STATE_WAITING_PLAYER;
    else if((irs[0] > (HOLD_LONG / USERINTERACTION_REFRESH_T))  && (irs[1] > (HOLD_LONG / USERINTERACTION_REFRESH_T)))
        gamestates = (gamestates & ~STATE_BITS) | STATE_PLAYING;
    chSemSignal(&gamestates_sem);
}

void ingame_check(uint8_t *irs) {
    chSemWait(&gamestates_sem);
    if((irs[0] > (HOLD_LONG / USERINTERACTION_REFRESH_T)) && (irs[1] > (HOLD_LONG / USERINTERACTION_REFRESH_T)))
        gamestates = (gamestates & ~STATE_BITS) | STATE_END;
    chSemSignal(&gamestates_sem);
}

void show_stateofgame(void) {
    chSemWait(&gamestates_sem);
    if((gamestates & DIFFICULTY_BITS) == DIFFICULTY_EASY) {
        // Set color to blue
    }
    else if((gamestates & DIFFICULTY_BITS) == DIFFICULTY_HARD) {
        // Set color to red
    }

    if((gamestates & STATE_BITS) == STATE_PLAYING)
        set_body_led(LED_ON);
    else if((gamestates & STATE_BITS) == STATE_WAITING_PLAYER)
        set_body_led(LED_OFF);
    chSemSignal(&gamestates_sem);
}