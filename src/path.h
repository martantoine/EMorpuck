/**
 * @file path.h
 * @author Antoine Martin (martantoine@tuta.io)
 * @brief 
 * @version 0.1
 * @date 2022-04-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef PATH_H
#define PATH_H

#include <stdint.h>

struct pos_t
{
    uint8_t x, y;
    uint16_t t;
};

struct pos_t* findPath(struct pos_t target, uint8_t* size);

#endif PATH_H