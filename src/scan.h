#ifndef SCAN_H
#define SCAN_H

#include <stdint.h>
#include "typedef.h"

void updateMap(cell_t **gameMap, coord_t *position);
uint8_t scanLine(void);

#endif /*SCAN_H*/