
#ifndef COLOR_H
#define COLOR_H


#include "camera/dcmi_camera.h"
#include <camera/po8030.h>
#include "constants.h"


uint8_t mean_center (uint8_t *buffer);
void process_image_start(void);
//void scanLine(color_t *c, uint *dist);
color_t scanColor(void);

#endif /* COLOR_H */