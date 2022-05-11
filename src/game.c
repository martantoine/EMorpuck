#include "game.h"
#include "constants.h"
#include "shared_var.h"
#include <stdlib.h>
#include <leds.h>
#include <ch.h>

semaphore_t gamestates_sem;
uint8_t gamestates;

semaphore_t gameMap_s;
cell_t gameMap[GAMEMAP_SIDE_NCELL][GAMEMAP_SIDE_NCELL];

coord_t play_center(cell_t **gameMap);

void gameMap_init(void) {
    //chSemObjectInit(&gameMap_s, 1);
    //chSemWait(&gameMap_s);
    for(uint8_t x = 0; x < GAMEMAP_SIDE_NCELL; x++)
        for(uint8_t y = 0; y < GAMEMAP_SIDE_NCELL; y++)
            gameMap[x][y] = (cell_t){
                .state = 0,
                .f_score = 0xFF,        // 0xFF is large enough to be always be greater than the one calculated
                .g_score = 0xFF,
                .parent = NULL
            };

    for(uint8_t i = 0; i < 12; i++) {
        if((i < 3) || ((i > 5) && (i < 9)))
            /*
             * Red cells are the one on the left and right sides
             */
            gameMap[storage[i].x][storage[i].y].state |= CELL_OCCUPED_RED;
        else
            /*
             * Blue cells are the one on the north and south sides
             */
            gameMap[storage[i].x][storage[i].y].state |= CELL_OCCUPED_BLUE;
    }
    //chSemSignal(&gameMap_s);
}

bool check_if_full(cell_t **gameMap) {
    for (int i = -1; i < 3; ++i)
        for (int j = -1; j < 3; ++j)
            if ((gameMap[i+GAMEMAP_CENTER][j+GAMEMAP_CENTER].state & OBSTRUCTION_BITS) == CELL_FREE)
                return false;
    return true;
}

line_t check_winning_condition(cell_t **gameMap) {
    int nb_of_red = 0;
    int nb_of_blue = 0;

    for (int i = -1 + GAMEMAP_CENTER; i < GAMEMAP_CENTER + 3; ++i) {
        /*vertical*/
        nb_of_blue = 0;
        nb_of_red = 0;
        for (int j = -1 + GAMEMAP_CENTER; j < GAMEMAP_CENTER + 3; j++) {
            if ((gameMap[i][j].state & OBSTRUCTION_BITS) == CELL_OCCUPED_BLUE)
                nb_of_blue += 1;
            else if((gameMap[i][j].state & OBSTRUCTION_BITS) == CELL_OCCUPED_RED)
                nb_of_red += 1;

            if (nb_of_red == 3)
                return RED_LINE;
            else if (nb_of_blue == 3)
                return BLUE_LINE;
        }

        /*horizontal*/
        nb_of_blue = 0;
        nb_of_red = 0;
        for (int j = -1 + GAMEMAP_CENTER; j < GAMEMAP_CENTER + 3; j++) {
            if ((gameMap[j][i].state & OBSTRUCTION_BITS) == CELL_OCCUPED_BLUE)
                nb_of_blue += 1;
            else if((gameMap[j][i].state & OBSTRUCTION_BITS) == CELL_OCCUPED_RED)
                nb_of_red += 1;

            if (nb_of_red == 3)
                return RED_LINE;
            else if (nb_of_blue == 3)
                return BLUE_LINE;
        }
    }

    /*diagonal*/
    if (((gameMap[0][0].state & OBSTRUCTION_BITS) == CELL_OCCUPED_BLUE) &&
        ((gameMap[1][1].state & OBSTRUCTION_BITS) == CELL_OCCUPED_BLUE) &&
        ((gameMap[2][2].state & OBSTRUCTION_BITS) == CELL_OCCUPED_BLUE))
    {
        return BLUE_LINE;
    }
    else if (((gameMap[0][0].state & OBSTRUCTION_BITS) == CELL_OCCUPED_RED) &&
             ((gameMap[1][1].state & OBSTRUCTION_BITS) == CELL_OCCUPED_RED) &&
             ((gameMap[2][2].state & OBSTRUCTION_BITS) == CELL_OCCUPED_RED))
    {
        return RED_LINE;
    }
    else if (((gameMap[0][2].state & OBSTRUCTION_BITS) == CELL_OCCUPED_BLUE) &&
             ((gameMap[1][1].state & OBSTRUCTION_BITS) == CELL_OCCUPED_BLUE) &&
             ((gameMap[2][0].state & OBSTRUCTION_BITS) == CELL_OCCUPED_BLUE))
    {
        return BLUE_LINE;
    }
    else if (((gameMap[0][2].state & OBSTRUCTION_BITS) == CELL_OCCUPED_RED) &&
             ((gameMap[1][1].state & OBSTRUCTION_BITS) == CELL_OCCUPED_RED) &&
             ((gameMap[2][0].state & OBSTRUCTION_BITS) == CELL_OCCUPED_RED))
    {
        return RED_LINE;
    }

    // You haven't won yet, you're bad aha
    return NO_LINE;
}

coord_t play_center(cell_t **gameMap) {
    if( gameMap[1][1].state& OBSTRUCTION_BITS== CELL_FREE )
        for(uint8_t w = 0; w < 4; w++)
            if((gameMap[GAMEMAP_CENTER + nearest[w].x][GAMEMAP_CENTER + nearest[w].y].state & OBSTRUCTION_BITS) == CELL_FREE)
                return (coord_t){.x=GAMEMAP_CENTER, .y=GAMEMAP_CENTER, .t=nearest[w].t};
    return (coord_t){.x=0, .y=0, .t=E};
}
coord_t place_easy(cell_t **gameMap) {
    //place the center first if not played by the player.
    coord_t tmp = play_center(gameMap);
    if((tmp.x == GAMEMAP_CENTER) && (tmp.y == GAMEMAP_CENTER))
        return tmp;

    for(;;) {
        //assign 2 random coordinates between 0 and 3
        //use of coord_t instead of 2 int because this struct takes 1 bytes instead of 2 bytes
        tmp = {
            .x = GAMEMAP_CENTER - 3 + (rand() % 4),
            .y = GAMEMAP_CENTER - 3 + (rand() % 4)
        };

        if((gameMap[tmp.x][tmp.y].state & OBSTRUCTION_BITS) == CELL_FREE)
            return tmp;
    }
}

coord_t place_hard(cell_t **gameMap) {
    /*
     * corners are stored twice more and the center is store three time more
     * that other cells are
     * this strategy is probabilistic based, is better than the true random strategy "play_easy"
     * however, it doesn't guarantee a 100% winning rate
     */
    const uint8_t table_probabilist[12][2] = {
        {0, 0},
        {0, 0},
        {0, 1},
        {0, 2},
        {0, 2},
        {1, 0},
        {1, 2},
        {2, 0},
        {2, 0},
        {2, 1},
        {2, 2},
        {2, 2}
    };

    coord_t tmp = play_center(gameMap);
    if((tmp.x == GAMEMAP_CENTER) && (tmp.y == GAMEMAP_CENTER))
        return tmp;

    for(;;) {
        uint8_t k = rand() % 13;
        tmp = {
            .x = table_probabilist[k][0],
            .y = table_probabilist[k][1]
        };
        if((gameMap[tmp.x][tmp.y].state & OBSTRUCTION_BITS) == CELL_FREE) {
            // add code to find angle of attack
            return tmp;
        }
    }
}

bool end_game(cell_t **gameMap) {
    if(check_if_3_alligned(gameMap) == RED_LINE) {
        //sets all rgb leds to RED
        clear_leds();
        set_rgb_led(0, 10, 0, 0);
        set_rgb_led(1, 10, 0, 0);
        set_rgb_led(2, 10, 0, 0);
        set_rgb_led(3, 10, 0, 0);
        return true;
    }
    if(check_if_3_alligned(gameMap) == BLUE_LINE) {
        //sets all rgb leds to RED
        clear_leds();
        set_rgb_led(0, 0, 0, 10);
        set_rgb_led(1, 0, 0, 10);
        set_rgb_led(2, 0, 0, 10);
        set_rgb_led(3, 0, 0, 10);
        return true;
    }
    return false;
}

void init_gamestates(void) {
    chSemObjectInit(&gamestates_sem, 1);
    chSemWait(&gamestates_sem);
    gamestates = 0;
    chSemSignal(&gamestates_sem);
}