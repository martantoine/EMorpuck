#ifndef PATH_H
#define PATH_H

#include "../defs.h"

step_t* findPath(cell_t **gameMap, coord_t current, const coord_t target);

#endif /*PATH_H*/