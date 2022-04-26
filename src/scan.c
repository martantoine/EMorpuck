#include "scan.h"

#include "shared_var.h"
#include "constants.h"

void scanMap(void)
{
    for(int8_t i = -1; i <= 1; i++)
        for(int8_t j = -1; j <= 1; j++)
            if(gameMap[i + GAMEMAP_CENTER_INDEX][j + GAMEMAP_CENTER_INDEX] == CELL_FREE)
            {
                gameMap[i + GAMEMAP_CENTER_INDEX][j + GAMEMAP_CENTER_INDEX] += CELL_2CHECK;
                //pathFind(gameMap[i + GAMEMAP_CENTER_INDEX][j + GAMEMAP_CENTER_INDEX]);
                chThdYield();
                scanLine();
            }
}