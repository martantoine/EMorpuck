/**
 * @file typedef.h
 * @author Antoine Martin (martantoine@tuta.io)
 * @brief 
 * @version 0.1
 * @date 2022-04-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef TYPEDEF_H
#define TYPEDEF_H

#include "hal.h"

/**
 * @brief Type used to designate cell position, can only take integer value
 * 
 */

/**
 * @brief Type used to designate the precise robot position, is a fixed point value
 * 32 bits : 8 bits to the left of the fixed point, 24 bits to the right of the fixed point 
 * (plenty enough of precision)
 */

/**
 * @brief Keep only the last 8 bits
 * 
 */
uint8_t roundValue(uint32_t value);
struct coord8_t roundCoord(struct coord32_t coord);

#endif