
#ifndef COLOR_H
#define COLOR_H


#include "camera/dcmi_camera.h"
#include <camera/po8030.h>


int mean_center (uint8_t *buffer);
void process_image_start(void);
#endif /* COLOR_H */