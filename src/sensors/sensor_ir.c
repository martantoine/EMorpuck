#include "sensor_ir.h"
#include "../defs.h"
#include "../game.h"
#include <ch.h>
#include <memory_protection.h>
#include <leds.h>
#include <sensors/proximity.h>
//#include <usbcfg.h>

inline void sensor_ir_measure(uint8_t *IRs);
inline void check_at_start(uint8_t *IRs);
inline void check_ingame(uint8_t *IRs);
inline void show_stateofgame(void);

static THD_WORKING_AREA(waSensorIR, 128);
static THD_FUNCTION(SensorIR, arg) {
    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    uint8_t ir_over_threshold[2];
    for(;;) {
        sensor_ir_measure(ir_over_threshold);
        chSemWait(&gamestates_sem);
        uint8_t gamestates_tmp = gamestates;
        chSemSignal(&gamestates_sem);
        if((gamestates_tmp & STATE_BITS) == STATE_START)
            check_at_start(ir_over_threshold);
        else if((gamestates_tmp & STATE_BITS) == STATE_WAITING_PLAYER)
            check_ingame(ir_over_threshold);
        else if((gamestates_tmp & STATE_BITS) == STATE_PLAYING)
            check_ingame(ir_over_threshold);
        show_stateofgame();
        chThdSleepMilliseconds(SENSOR_IR_REFRESH_T);
    }
}

void sensor_ir_init(void) {
    chSemObjectInit(&gamestates_sem, 1);
    proximity_start();
	chThdCreateStatic(waSensorIR, sizeof(waSensorIR), SENSOR_IR_PRIORITY, SensorIR, NULL);
    calibrate_ir();
}

void sensor_ir_measure(uint8_t *IRs) {
    chSemWait(&gamestates_sem);
    if(get_calibrated_prox(IR_1) > SENSOR_IR_THRESHOLD)
        IRs[0] = (IRs[0] < 0xFF) ? IRs[0] + 1 : IRs[0];
    else
        IRs[0] = 0;
    if(get_calibrated_prox(IR_2) > SENSOR_IR_THRESHOLD)
        IRs[1] = (IRs[1] < 0xFF) ? IRs[1] + 1 : IRs[1];
    else
        IRs[1] = 0;
    chSemSignal(&gamestates_sem);
}

void check_at_start(uint8_t *IRs) {
    chSemWait(&gamestates_sem);
    if     ((IRs[0] > (HOLD_SHORT / SENSOR_IR_REFRESH_T)) && (IRs[1] < (HOLD_SHORT / SENSOR_IR_REFRESH_T)))
        gamestates = (gamestates & ~DIFFICULTY_BITS) | DIFFICULTY_EASY;
    else if((IRs[0] < (HOLD_SHORT / SENSOR_IR_REFRESH_T)) && (IRs[1] > (HOLD_SHORT / SENSOR_IR_REFRESH_T)))
        gamestates = (gamestates & ~DIFFICULTY_BITS) | DIFFICULTY_HARD;
    else if((IRs[0] > (HOLD_SHORT / SENSOR_IR_REFRESH_T)) && (IRs[1] > (HOLD_SHORT / SENSOR_IR_REFRESH_T)))
        gamestates = (gamestates & ~STATE_BITS) | STATE_WAITING_PLAYER;
    else if((IRs[0] > (HOLD_LONG / SENSOR_IR_REFRESH_T))  && (IRs[1] > (HOLD_LONG / SENSOR_IR_REFRESH_T)))
        gamestates = (gamestates & ~STATE_BITS) | STATE_PLAYING;
    chSemSignal(&gamestates_sem);
}

void check_ingame(uint8_t *IRs) {
    chSemWait(&gamestates_sem);
    if((IRs[0] > (HOLD_LONG / SENSOR_IR_REFRESH_T)) && (IRs[1] > (HOLD_LONG / SENSOR_IR_REFRESH_T)))
        gamestates = (gamestates & ~STATE_BITS) | STATE_END;
    chSemSignal(&gamestates_sem);
}

void show_stateofgame(void) {
    //Do smth about a color code, it must be crystal clear
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