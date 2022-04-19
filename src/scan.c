#include "scan.h"
//#include "motor.h"
#include "main.h"

#define BLANK 0
#define CHECKED (-10)
#define TOCHECK 10

static uint8_t gameMap[3][3];

void initGameMap()
{
    uint8_t i = 0, j =0;
    for(;i < 3; i++)
        for(;j < 3; j++)
            gameMap[i][j] = BLANK + TOCHECK;
}

void ScanMap()
{
    bool done = false;
    float dist;
    char color;
    while(!done)
    {
        goto(pos_x, pos_y, );
        uint8_t color = BLANK;
        uint8_t dist;
        scanLine(dist, color);
        char i=1;
        for(i <= dist; i++)
        {
            switch (theta)
            {
            case 0 :
                if(i == dist)
                    gameMap[pos_x+i, pos_y] = BLANK + CHECKED;
                else
                    gameMap[pos_x+i, pos_y] = color + CHECKED;

                break;
            case 90 :
                break;
            case 180 :
                break;
            case 270 :
                break;
            }
        }
    }
}