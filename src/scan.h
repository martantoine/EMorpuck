#ifndef SCAN_H
#define SCAN_H

#include "defs.h"

void updateMap(cell_t gameMap[SIDE_NCELL][SIDE_NCELL], coord_t *position);
uint8_t scanLine(void);

#endif /*SCAN_H*/