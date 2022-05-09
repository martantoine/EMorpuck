/**
 * @file distance.h
 * @author Alexandre Duval(alduval0305@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef DISTANCE_H
#define DISTANCE_H


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

#include "sensors\VL53L0X\VL53L0X.h"
#include "leds.h"
#include "shared_var.h"
#include "constante.h"

void distance_start(void);
distnorm_t scanDistance(void);

extern uint16_t d_mm;
extern distnorm_t d;
#endif /*DISTANCE_H*/