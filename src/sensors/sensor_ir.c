#include "sensor_ir.h"
#include "../defs.h"
#include "../game.h"
#include <ch.h>
#include <hal.h>
#include <memory_protection.h>
#include <leds.h>
#include <sensors/proximity.h>

inline void sensor_ir_measure(uint8_t *ir_count_1, uint8_t *ir_count_2, uint8_t *ir_state_1, uint8_t *ir_state_2);
inline void check_at_start(uint8_t *ir_count_1, uint8_t *ir_count_2, uint8_t *ir_state_1, uint8_t *ir_state_2);
inline void check_ingame(uint8_t *ir_count_1, uint8_t *ir_count_2, uint8_t *ir_state_1, uint8_t *ir_state_2);
inline void show_stateofgame(void);

static THD_WORKING_AREA(waSensorIR, 256);
static THD_FUNCTION(SensorIR, arg) {
    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    uint8_t ir_count_1=0;
    uint8_t ir_count_2=0;
    uint8_t ir_state_1=0;
    uint8_t ir_state_2=0;
    systime_t time;
    while(1) {
        chSemWait(&gamestates_sem);
        sensor_ir_measure(&ir_count_1, &ir_count_2, &ir_state_1, &ir_state_2);
        switch(gamestates & STATE_BITS) {
            case STATE_START:
                check_at_start(&ir_count_1, &ir_count_2, &ir_state_1, &ir_state_2); break;
            case STATE_WAITING_PLAYER:
                check_ingame(&ir_count_1, &ir_count_2, &ir_state_1, &ir_state_2); break;
            case STATE_PLAYING:
                check_ingame(&ir_count_1, &ir_count_2, &ir_state_1, &ir_state_2); break;
            default:
                break;
        }
        //show_stateofgame();
        chSemSignal(&gamestates_sem);

        palTogglePad(GPIOD, GPIOD_LED_FRONT);
        time = chVTGetSystemTime();
        chThdSleepUntilWindowed(time, time + MS2ST(SENSOR_IR_REFRESH_T));
    }
}

void sensor_ir_init(void) {
    //spi_comm_start();
    proximity_start();
    calibrate_ir();
	chThdCreateStatic(waSensorIR, sizeof(waSensorIR), NORMALPRIO+1, SensorIR, NULL);
}

void sensor_ir_measure(uint8_t *ir_count_1, uint8_t *ir_count_2, uint8_t *ir_state_1, uint8_t *ir_state_2) {
    if(*ir_state_1 == 1) {
        *ir_state_1 = 2;
        *ir_count_1 = 0;
    }
    if(get_calibrated_prox(3) > SENSOR_IR_THRESHOLD) {
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
        if     ((*ir_count_1 > (HOLD_LONG / SENSOR_IR_REFRESH_T))  && (*ir_count_2 < (HOLD_LONG / SENSOR_IR_REFRESH_T)))
            gamestates = (gamestates & ~DIFFICULTY_BITS) | DIFFICULTY_EASY;
        else if((*ir_count_1 > (HOLD_SHORT / SENSOR_IR_REFRESH_T)) && (*ir_count_2 < (HOLD_SHORT / SENSOR_IR_REFRESH_T)))
            gamestates = (gamestates & ~DIFFICULTY_BITS) | DIFFICULTY_HARD;
        else if((*ir_count_1 < (HOLD_LONG / SENSOR_IR_REFRESH_T))  && (*ir_count_2 > (HOLD_LONG / SENSOR_IR_REFRESH_T)))
            gamestates = (gamestates & ~STATE_BITS) | STATE_WAITING_PLAYER;
        else if((*ir_count_1 < (HOLD_SHORT / SENSOR_IR_REFRESH_T)) && (*ir_count_2 > (HOLD_SHORT / SENSOR_IR_REFRESH_T)))
            gamestates = (gamestates & ~STATE_BITS) | STATE_PLAYING;
    }
}

void check_ingame(uint8_t *ir_count_1, uint8_t *ir_count_2, uint8_t *ir_state_1, uint8_t *ir_state_2) {
    if((*ir_state_1 == 1) || (*ir_state_2 == 1)) {
        if((*ir_count_1 > (HOLD_LONG / SENSOR_IR_REFRESH_T)) && (*ir_count_2 > (HOLD_LONG / SENSOR_IR_REFRESH_T)))
            gamestates = (gamestates & ~STATE_BITS) | STATE_END;
        else if((*ir_count_1 > (HOLD_SHORT / SENSOR_IR_REFRESH_T)) && (*ir_count_2 < (HOLD_SHORT / SENSOR_IR_REFRESH_T)))
            if((gamestates & STATE_BITS) | STATE_WAITING_PLAYER)
                gamestates = (gamestates & ~STATE_BITS) | STATE_PLAYING;
    }
}

void toggle_rgb_leds(rgb_led_name_t led_number, uint8_t red_val, uint8_t green_val, uint8_t blue_val) {
    static int b = 0;
    if(b==0) {
        set_rgb_led(led_number, red_val, green_val, blue_val);
        b = 1;
    }
    else {
        set_rgb_led(led_number, 0, 0, 0);
        b = 0;
    }
}

void show_stateofgame(void) {
    if((gamestates & STATE_BITS) != STATE_END) {
        clear_leds();
        if((gamestates & DIFFICULTY_BITS) == DIFFICULTY_EASY) {
            set_rgb_led(LED8,0,0,0xFF);
            set_rgb_led(LED2,0,0,0xFF);
        }
        else if((gamestates & DIFFICULTY_BITS) == DIFFICULTY_HARD) {
            set_rgb_led(LED8,0xFF,0,0);
            set_rgb_led(LED2,0xFF,0,0);
        }
    }
    if((gamestates & STATE_BITS) == STATE_START) {
        set_rgb_led(LED4,0,0xFF,0);
        set_rgb_led(LED6,0,0xFF,0);
    }
    else if((gamestates & STATE_BITS) == STATE_PLAYING) {
        set_rgb_led(LED4,0xFF,0,0);
        set_rgb_led(LED6,0xFF,0,0);
        if((gamestates & SCANNING_BITS) == SCANNING_RED) {
            set_rgb_led(LED2,0xFF,0,0);
            set_rgb_led(LED4,0xFF,0,0);
            set_rgb_led(LED6,0xFF,0,0);
            set_rgb_led(LED8,0xFF,0,0);
        }
        else if((gamestates & SCANNING_BITS) == SCANNING_BLUE) {
            set_rgb_led(LED2,0,0,0xFF);
            set_rgb_led(LED4,0,0,0xFF);
            set_rgb_led(LED6,0,0,0xFF);
            set_rgb_led(LED8,0,0,0xFF);
        }
        else if((gamestates & SCANNING_BITS) == SCANNING_NONE) {
            set_rgb_led(LED2,0xFF,0xFF,0xFF);
            set_rgb_led(LED4,0xFF,0xFF,0xFF);
            set_rgb_led(LED6,0xFF,0xFF,0xFF);
            set_rgb_led(LED8,0xFF,0xFF,0xFF);
        }
    }
    else if((gamestates & STATE_BITS) == STATE_WAITING_PLAYER) {
        set_rgb_led(LED4,0,0,0xFF);
        set_rgb_led(LED6,0,0,0xFF);
    }
    else if((gamestates & STATE_BITS) == STATE_END) {
        if((gamestates & WINNER_BITS) == WINNER_RED) {
            toggle_rgb_leds(LED2,0xFF,0,0);
            toggle_rgb_leds(LED4,0xFF,0,0);
            toggle_rgb_leds(LED6,0xFF,0,0);
            toggle_rgb_leds(LED8,0xFF,0,0);
        }
        else if((gamestates & WINNER_BITS) == WINNER_BLUE) {
            toggle_rgb_leds(LED2,0,0,0xFF);
            toggle_rgb_leds(LED4,0,0,0xFF);
            toggle_rgb_leds(LED6,0,0,0xFF);
            toggle_rgb_leds(LED8,0,0,0xFF);
        }
        else if((gamestates & WINNER_BITS) == WINNER_DRAW) {
            toggle_rgb_leds(LED2,0xFF,0xFF,0xFF);
            toggle_rgb_leds(LED4,0xFF,0xFF,0xFF);
            toggle_rgb_leds(LED6,0xFF,0xFF,0xFF);
            toggle_rgb_leds(LED8,0xFF,0xFF,0xFF);
        }
    }
}