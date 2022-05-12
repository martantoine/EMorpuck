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

static THD_WORKING_AREA(waSensorIR, 256);
static THD_FUNCTION(SensorIR, arg) {
    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    uint8_t ir_over_threshold[2];
    for(;;) {
        chSemWait(&gamestates_sem);

        sensor_ir_measure(ir_over_threshold);
        if((gamestates & STATE_BITS) == STATE_START)
            check_at_start(ir_over_threshold);
        else if((gamestates & STATE_BITS) == STATE_WAITING_PLAYER)
            check_ingame(ir_over_threshold);
        else if((gamestates & STATE_BITS) == STATE_PLAYING)
            check_ingame(ir_over_threshold);
        show_stateofgame();
        
        chSemSignal(&gamestates_sem);
        chThdSleepMilliseconds(SENSOR_IR_REFRESH_T);
    }
}

void sensor_ir_init(void) {
    chSemObjectInit(&gamestates_sem, 1);
    gamestates = 0;
    proximity_start();
	chThdCreateStatic(waSensorIR, sizeof(waSensorIR), NORMALPRIO+1, SensorIR, NULL);
    calibrate_ir();
}

void sensor_ir_measure(uint8_t *IRs) {
    if(get_calibrated_prox(3) < SENSOR_IR_THRESHOLD)
        IRs[0] = (IRs[0] < 0xFF) ? (IRs[0] + 1) : IRs[0];
    else
        IRs[0] = 0;
    if(get_calibrated_prox(6) < SENSOR_IR_THRESHOLD)
        IRs[1] = (IRs[1] < 0xFF) ? (IRs[1] + 1) : IRs[1];
    else
        IRs[1] = 0;
}

void check_at_start(uint8_t *IRs) {
    if     ((IRs[0] > (HOLD_SHORT / SENSOR_IR_REFRESH_T)) && (IRs[1] < (HOLD_SHORT / SENSOR_IR_REFRESH_T)))
        gamestates = (gamestates & ~DIFFICULTY_BITS) | DIFFICULTY_EASY;
    else if((IRs[0] < (HOLD_SHORT / SENSOR_IR_REFRESH_T)) && (IRs[1] > (HOLD_SHORT / SENSOR_IR_REFRESH_T)))
        gamestates = (gamestates & ~DIFFICULTY_BITS) | DIFFICULTY_HARD;
    else if((IRs[0] > (HOLD_SHORT / SENSOR_IR_REFRESH_T)) && (IRs[1] > (HOLD_SHORT / SENSOR_IR_REFRESH_T)))
        gamestates = (gamestates & ~STATE_BITS) | STATE_WAITING_PLAYER;
    else if((IRs[0] > (HOLD_LONG / SENSOR_IR_REFRESH_T))  && (IRs[1] > (HOLD_LONG / SENSOR_IR_REFRESH_T)))
        gamestates = (gamestates & ~STATE_BITS) | STATE_PLAYING;
}

void check_ingame(uint8_t *IRs) {
    if((IRs[0] > (HOLD_LONG / SENSOR_IR_REFRESH_T)) && (IRs[1] > (HOLD_LONG / SENSOR_IR_REFRESH_T)))
        gamestates = (gamestates & ~STATE_BITS) | STATE_END;
}

void show_stateofgame(void) {
    clear_leds();
    //Do smth about a color code, it must be crystal clear
    if((gamestates & DIFFICULTY_BITS) == DIFFICULTY_EASY) {
        // Set color to blue
        set_rgb_led(LED8,0,0,10);
        set_rgb_led(LED2,0,0,10);
    }
    else if((gamestates & DIFFICULTY_BITS) == DIFFICULTY_HARD) {
        // Set color to red
        set_rgb_led(LED8,10,0,0);
        //set_rgb_led(LED2,10,0,0);
    }
    if((gamestates & STATE_BITS) == STATE_START) {
        set_rgb_led(LED4,0,0xFF,0);
        set_rgb_led(LED6,0,0xFF,0);
    }
    else if((gamestates & STATE_BITS) == STATE_PLAYING) {
        set_rgb_led(LED4,0xFF,0,0);
        set_rgb_led(LED6,0xFF,0,0);
    }
    else if((gamestates & STATE_BITS) == STATE_WAITING_PLAYER) {
        set_rgb_led(LED4,0,0,0xFF);
        set_rgb_led(LED6,0,0,0xFF);
    }
}