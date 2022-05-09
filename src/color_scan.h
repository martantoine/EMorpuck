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


#include "camera/dcmi_camera.h"
#include <camera/po8030.h>
#include "constants.h"


uint8_t mean_center (uint8_t *buffer);
void process_image_start(void);
//void scanLine(color_t *c, uint *dist);
color_t scanColor(void);
extern  color_t c;
#endif /* COLOR_H */