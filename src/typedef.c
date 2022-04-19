/**
 * @file typedef.c
 * @author Antoine Martin (martantoine@tuta.io)
 * @brief 
 * @version 0.1
 * @date 2022-04-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "typedef.h"
//#include "motor.h"

uint8_t roundValue(uint32_t value)
{
    return (value >> 24);
}

struct coord8_t roundCoord(struct coord32_t coord)
{
    struct coord8_t coord2 = {roundValue(coord.x), roundValue(coord.y) };
    return coord2;
}