#include "shared_var.h"

void init_position()
{
    chSemObjectInit(&position_s, 1);
    position_x = 0;
    position_y = 0;
    position_t = 0;
}

void init_path()
{
    chSemObjectInit(&path_s, 1);
    path = NULL;
}