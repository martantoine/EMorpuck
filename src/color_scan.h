/**
 * @file color_scan.h
 * @author Alexandre Duval(alduval0305@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-05-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef COLOR_H
#define COLOR_H

#include "typedef.h"

extern  color_t c;

void color_init(void);
color_t scanColor(void);

#endif /* COLOR_H */