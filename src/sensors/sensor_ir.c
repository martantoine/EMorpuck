#include "sensor_ir.h"
#include "../defs.h"
#include "../game.h"
#include <ch.h>
#include <memory_protection.h>
#include <leds.h>
#include <sensors/proximity.h>
//#include <usbcfg.h>

inline void sensor_ir_measure(uint8_t *ir_count_1, uint8_t *ir_count_2, uint8_t *ir_state_1, uint8_t *ir_state_2);
inline void check_at_start(uint8_t *ir_count_1, uint8_t *ir_count_2, uint8_t *ir_state_1, uint8_t *ir_state_2);
inline void check_ingame(uint8_t *ir_count_1, uint8_t *ir_count_2);
inline void show_stateofgame(void);

static THD_WORKING_AREA(waSensorIR, 256);
static THD_FUNCTION(SensorIR, arg) {
    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    uint8_t ir_count_1=0, ir_count_2=0;
    uint8_t ir_state_1=0, ir_state_2=0;
    systime_t time;
    for(;;) {
        //ir_count_1 = get_calibrated_prox(3);

        chSemWait(&gamestates_sem);
        
        sensor_ir_measure(&ir_count_1, &ir_count_2, &ir_state_1, &ir_state_2);
        if((gamestates & STATE_BITS) == STATE_START)
            check_at_start(&ir_count_1, &ir_count_2, &ir_state_1, &ir_state_2);
        else if((gamestates & STATE_BITS) == STATE_WAITING_PLAYER)
            check_ingame(&ir_count_1, &ir_count_2);
        else if((gamestates & STATE_BITS) == STATE_PLAYING)
            check_ingame(&ir_count_1, &ir_count_2);
        show_stateofgame();

        chSemSignal(&gamestates_sem);
        time = chVTGetSystemTime();
        chThdSleepUntilWindowed(time, time + MS2ST(SENSOR_IR_REFRESH_T));
    }
}

void sensor_ir_init(void) {
    chSemObjectInit(&gamestates_sem, 1);
    gamestates = 0;
    proximity_start();
	chThdCreateStatic(waSensorIR, sizeof(waSensorIR), NORMALPRIO+1, SensorIR, NULL);
    calibrate_ir();
}

void sensor_ir_measure(uint8_t *ir_count_1, uint8_t *ir_count_2, uint8_t *ir_state_1, uint8_t *ir_state_2) {
    if(*ir_state_1 == 1) {
        *ir_state_1 = 2;
        *ir_count_1 = 0;
    }
    if(get_calibrated_prox(2) > SENSOR_IR_THRESHOLD) {
        *ir_count_1 = (*ir_count_1 < 0xFF) ? (*ir_count_1 + 1) : *ir_count_1;
        *ir_state_1 = 0;
    }
    else {
        *ir_state_1 = 1;
    }
    if(*ir_state_2 == 1) {
        *ir_state_2 = 2;
        *ir_count_2 = 0;
    }
    if(get_calibrated_prox(6) > SENSOR_IR_THRESHOLD) {
        *ir_count_2 = (*ir_count_2 < 0xFF) ? (*ir_count_2 + 1) : *ir_count_2;
        *ir_state_2 = 0;
    }
    else {
        *ir_state_2 = 1;
    }
}

void check_at_start(uint8_t *ir_count_1, uint8_t *ir_count_2, uint8_t *ir_state_1, uint8_t *ir_state_2) {
    if((*ir_state_1 == 1) && (*ir_state_2 == 1)) {
        if     ((*ir_count_1 > (HOLD_LONG / SENSOR_IR_REFRESH_T)) && (*ir_count_2 < (HOLD_LONG / SENSOR_IR_REFRESH_T)))
        {
            gamestates = (gamestates & ~DIFFICULTY_BITS) | DIFFICULTY_EASY;
            //*ir_count_1 = 0;
            //*ir_count_2 = 0;
        }
        else if((*ir_count_1 > (HOLD_SHORT / SENSOR_IR_REFRESH_T)) && (*ir_count_2 < (HOLD_SHORT / SENSOR_IR_REFRESH_T)))
        {
            gamestates = (gamestates & ~DIFFICULTY_BITS) | DIFFICULTY_HARD;
            //*ir_count_1 = 0;
            //*ir_count_2 = 0;
        }
        else if((*ir_count_1 < (HOLD_LONG / SENSOR_IR_REFRESH_T)) && (*ir_count_2 > (HOLD_LONG / SENSOR_IR_REFRESH_T)))
        {   gamestates = (gamestates & ~STATE_BITS) | STATE_WAITING_PLAYER;
            //*ir_count_1 = 0;
            //*ir_count_2 = 0;
        }
        else if((*ir_count_1 < (HOLD_SHORT / SENSOR_IR_REFRESH_T))  && (*ir_count_2 > (HOLD_SHORT / SENSOR_IR_REFRESH_T)))
        {
            gamestates = (gamestates & ~STATE_BITS) | STATE_PLAYING;
            //*ir_count_1 = 0;
            //*ir_count_2 = 0;
        }
    }
}

void check_ingame(uint8_t *ir_count_1, uint8_t *ir_count_2) {
    if((*ir_count_1 > (HOLD_LONG / SENSOR_IR_REFRESH_T)) && (*ir_count_2 > (HOLD_LONG / SENSOR_IR_REFRESH_T)))
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
        set_rgb_led(LED2,10,0,0);
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