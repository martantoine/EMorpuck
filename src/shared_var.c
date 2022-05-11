#include "shared_var.h"
#include "constants.h"

const coord_t nearest[4] = {
    {-1,  0, E},
    {+1,  0, W},
    { 0, -1, S},
    { 0, +1, N}
};

const coord_t storage[12] = {
    { GAMEMAP_CENTER - 3, GAMEMAP_CENTER - 1, E},
    { GAMEMAP_CENTER - 3, GAMEMAP_CENTER    , E},
    { GAMEMAP_CENTER - 3, GAMEMAP_CENTER + 1, E},

    { GAMEMAP_CENTER + 3, GAMEMAP_CENTER - 1, W},
    { GAMEMAP_CENTER + 3, GAMEMAP_CENTER    , W},
    { GAMEMAP_CENTER + 3, GAMEMAP_CENTER + 1, W},

    { GAMEMAP_CENTER - 1, GAMEMAP_CENTER - 3, S},
    { GAMEMAP_CENTER    , GAMEMAP_CENTER - 3, S},
    { GAMEMAP_CENTER + 1, GAMEMAP_CENTER - 3, S},

    { GAMEMAP_CENTER - 1, GAMEMAP_CENTER + 3, N},
    { GAMEMAP_CENTER    , GAMEMAP_CENTER + 3, N},
    { GAMEMAP_CENTER + 1, GAMEMAP_CENTER + 3, N}
};