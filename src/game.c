#include "game.h"
#include "defs.h"
#include <stdlib.h>
#include <leds.h>
#include <ch.h>

semaphore_t gamestates_sem;
volatile uint8_t gamestates;

void game_init(cell_t gameMap[SIDE_NCELL][SIDE_NCELL]) {
    for(uint8_t x = 0; x < SIDE_NCELL; x++)
        for(uint8_t y = 0; y < SIDE_NCELL; y++)
            gameMap[x][y] = (cell_t){
                .state = 0,
                .f_score = 0xFF, // 0xFF is large enough to be always be greater than the one calculated
                .g_score = 0xFF,
                .parent = NULL
            };

    for(uint8_t i = 0; i < 12; i++) {
        if((i < 3) || ((i > 5) && (i < 9)))
            // Red cells are the one on the left and right sides
            gameMap[storage[i].x][storage[i].y].state |= CELL_OCCUPED_RED;
        else
            // Blue cells are the one on the north and south sides
            gameMap[storage[i].x][storage[i].y].state |= CELL_OCCUPED_BLUE;
    }

    chSemObjectInit(&gamestates_sem, 1);
    chSemWait(&gamestates_sem);
    gamestates = 0;
    chSemSignal(&gamestates_sem);
}

winner_t search_winner(cell_t gameMap[SIDE_NCELL][SIDE_NCELL]) {
    int nb_of_red = 0;
    int nb_of_blue = 0;

    for (int x = -1 + SIDE_NCELL; x < SIDE_NCELL + 3; x++) {
        /*vertical*/
        nb_of_blue = 0;
        nb_of_red = 0;
        for (int y = -1 + SIDE_NCELL; y < SIDE_NCELL + 3; y++) {
            if ((gameMap[x][y].state & OBSTRUCTION_BITS) == CELL_OCCUPED_BLUE)
                nb_of_blue++;
            else if((gameMap[x][y].state & OBSTRUCTION_BITS) == CELL_OCCUPED_RED)
                nb_of_red++;

            if (nb_of_red == 3)
                return PLAYER_RED;
            else if (nb_of_blue == 3)
                return PLAYER_BLUE;
        }

        /*horizontal*/
        nb_of_blue = 0;
        nb_of_red = 0;
        for (int y = -1 + SIDE_NCELL; y < SIDE_NCELL + 3; y++) {
            if ((gameMap[y][x].state & OBSTRUCTION_BITS) == CELL_OCCUPED_BLUE)
                nb_of_blue++;
            else if((gameMap[y][x].state & OBSTRUCTION_BITS) == CELL_OCCUPED_RED)
                nb_of_red++;

            if (nb_of_red == 3)
                return PLAYER_RED;
            else if (nb_of_blue == 3)
                return PLAYER_BLUE;
        }
    }

    /*diagonal*/
    if (((gameMap[0][0].state & OBSTRUCTION_BITS) == CELL_OCCUPED_BLUE) &&
        ((gameMap[1][1].state & OBSTRUCTION_BITS) == CELL_OCCUPED_BLUE) &&
        ((gameMap[2][2].state & OBSTRUCTION_BITS) == CELL_OCCUPED_BLUE))
    {
        return PLAYER_BLUE;
    }
    else if (((gameMap[0][0].state & OBSTRUCTION_BITS) == CELL_OCCUPED_RED) &&
             ((gameMap[1][1].state & OBSTRUCTION_BITS) == CELL_OCCUPED_RED) &&
             ((gameMap[2][2].state & OBSTRUCTION_BITS) == CELL_OCCUPED_RED))
    {
        return PLAYER_RED;
    }
    else if (((gameMap[0][2].state & OBSTRUCTION_BITS) == CELL_OCCUPED_BLUE) &&
             ((gameMap[1][1].state & OBSTRUCTION_BITS) == CELL_OCCUPED_BLUE) &&
             ((gameMap[2][0].state & OBSTRUCTION_BITS) == CELL_OCCUPED_BLUE))
    {
        return PLAYER_BLUE;
    }
    else if (((gameMap[0][2].state & OBSTRUCTION_BITS) == CELL_OCCUPED_RED) &&
             ((gameMap[1][1].state & OBSTRUCTION_BITS) == CELL_OCCUPED_RED) &&
             ((gameMap[2][0].state & OBSTRUCTION_BITS) == CELL_OCCUPED_RED))
    {
        return PLAYER_RED;
    }

    bool full = true;
    for (int x = -1; x < 3; x++)
        for (int y = -1; y < 3; y++)
            if ((gameMap[x + SIDE_NCELL][y + SIDE_NCELL].state & OBSTRUCTION_BITS) == CELL_FREE)
                full = false;
    if(full == true)
        return DRAW;
    return NONE;
}

coord_t play_center(cell_t gameMap[SIDE_NCELL][SIDE_NCELL]) {
    if((gameMap[GAMEMAP_CENTER][GAMEMAP_CENTER].state & OBSTRUCTION_BITS) == CELL_FREE)
        for(uint8_t w = 0; w < 4; w++) 
            if((gameMap[GAMEMAP_CENTER + nearest[w].x][GAMEMAP_CENTER + nearest[w].y].state & OBSTRUCTION_BITS) == CELL_FREE)
                return (coord_t){.x=GAMEMAP_CENTER, .y=GAMEMAP_CENTER, .t=nearest[w].t};
    return (coord_t){.x=0, .y=0, .t=E};
}

coord_t place_easy(cell_t gameMap[SIDE_NCELL][SIDE_NCELL]) {
    //place the center first if not played by the player.
    coord_t tmp = play_center(gameMap);
    if((tmp.x == GAMEMAP_CENTER) && (tmp.y == GAMEMAP_CENTER))
        return tmp;

    for(;;) {
        //assign 2 random coordinates between 0 and 3
        //use of coord_t instead of 2 int because this struct takes 1 bytes instead of 2 bytes
        tmp = (coord_t) {
            .x = SIDE_NCELL - 3 + (rand() % 4),
            .y = SIDE_NCELL - 3 + (rand() % 4)
        };

        if((gameMap[tmp.x][tmp.y].state & OBSTRUCTION_BITS) == CELL_FREE)
            return tmp;
    }
}

coord_t place_hard(cell_t gameMap[SIDE_NCELL][SIDE_NCELL]) {
    /*
     * corners are stored twice more and the center is store three time more
     * that other cells are
     * this strategy is probabilistic based, is better than the true random strategy "play_easy"
     * however, it doesn't guarantee a 100% winning rate
     */
    const uint8_t table_probabilist[12][2] = {
        {0, 0}, {0, 0},
        {0, 1},
        {0, 2}, {0, 2},
        {1, 0},
        {1, 2},
        {2, 0}, {2, 0},
        {2, 1},
        {2, 2}, {2, 2}
    };

    coord_t tmp = play_center(gameMap);
    if((tmp.x == GAMEMAP_CENTER) && (tmp.y == GAMEMAP_CENTER))
        return tmp;

    for(;;) {
        uint8_t k = rand() % 13;
        tmp = (coord_t) {
            .x = table_probabilist[k][0],
            .y = table_probabilist[k][1]
        };
        if((gameMap[tmp.x][tmp.y].state & OBSTRUCTION_BITS) == CELL_FREE) {
            // add code to find angle of attack
            return tmp;
        }
    }
}

void check_end_game(cell_t gameMap[SIDE_NCELL][SIDE_NCELL]) {
    switch(search_winner(gameMap)) {
        case PLAYER_RED:
            chSemWait(&gamestates_sem);
            gamestates = (gamestates & ~STATE_BITS) | STATE_END;
            chSemSignal(&gamestates_sem);
            /*clear_leds();
            set_rgb_led(0, 10, 0, 0);
            set_rgb_led(1, 10, 0, 0);
            set_rgb_led(2, 10, 0, 0);
            set_rgb_led(3, 10, 0, 0);*/
            break;
        case PLAYER_BLUE:
            chSemWait(&gamestates_sem);
            gamestates = (gamestates & ~STATE_BITS) | STATE_END;
            chSemSignal(&gamestates_sem);
            /*clear_leds();
            set_rgb_led(0, 0, 0, 10);
            set_rgb_led(1, 0, 0, 10);
            set_rgb_led(2, 0, 0, 10);
            set_rgb_led(3, 0, 0, 10);*/
            break;
        case DRAW:
            chSemWait(&gamestates_sem);
            gamestates = (gamestates & ~STATE_BITS) | STATE_END;
            chSemSignal(&gamestates_sem);
            /*clear_leds();
            set_rgb_led(0, 10, 10, 10);
            set_rgb_led(1, 10, 10, 10);
            set_rgb_led(2, 10, 10, 10);
            set_rgb_led(3, 10, 10, 10);*/
            break;
        case NONE:
            break;
    }
}