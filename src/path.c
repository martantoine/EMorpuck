²#include "path.h"
#include <stdlib.h>

struct pos_t* findPath(struct pos_t target, uint8_t* size)
{
    *size = 10;
    struct pos_t* path = malloc(sizeof(struct pos_t)*(*size));
    for(uint8_t i=0; i < 10; i++)
    {
        path[i].x = i;
        path[i].y = i;
        path[i].t = i;
    }    
    return path;
}