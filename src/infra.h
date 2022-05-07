/**
 * @file infra.h
 * @author Alexandre Duval(alduval0305@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef INFRA_H
#define INFRA_H


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


void ir_button_start(void);
void if_close_turn_on_led(void);
void launch_test(void);
void ir_button_check(void);
void show_stateofgame(void);

#endif /*INFRA_H*/