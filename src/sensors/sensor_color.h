#ifndef SENSOR_COLOR_H
#define SENSOR_COLOR_H

#include "../defs.h"

extern  color_t c;

void sensor_color_init(void);
color_t sensor_measure_color(void);

#endif /*SENSOR_COLOR_H*/