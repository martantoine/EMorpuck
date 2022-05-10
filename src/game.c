#include "game.h"
#include "constants.h"
#include <stdlib.h>
#include <time.h>
#include <leds.h>

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

void place_easy(cell_t **gameMap){
    for(;;) {
        srand(time(0)); //init should be done only once (separate function called by main)
        //assign 2 random coordinates between 0 and 3
        //use of coord_t instead of 2 int because this struct takes 1 bytes instead of 2 bytes
        coord_t tmp = {
            .x = GAMEMAP_CENTER - 3 + (rand() % 4),
            .y = GAMEMAP_CENTER - 3 + (rand() % 4)
        };

        if((gameMap[tmp.x][tmp.y].state & OBSTRUCTION_BITS) == CELL_FREE) {
            gameMap[tmp.x][tmp.y].state |= CELL_OCCUPED_RED;
            break;
        }
    }
}

void place_hard(cell_t **gameMap) {
    /*
     * corners are stored twice more and the center is store three time more
     * that other cells are
     * this strategy is probabilistic based, is better than the true random strategy "play_easy"
     * however, it doesn't guarantee a 100% winning rate
     */
    const uint8_t table_probabilist[15][2] = {
        {0, 0},
        {0, 0},
        {0, 1},
        {0, 2},
        {0, 2},
        {1, 0},
        {1, 1},
        {1, 1},
        {1, 1},
        {1, 2},
        {2, 0},
        {2, 0},
        {2, 1},
        {2, 2},
        {2, 2}
    };

    for(;;) {
        srand(time(0));
        uint8_t k = rand() % 16; // assign 1 random number between 0 and 3
        uint8_t i = table_probabilist[k][0];
        uint8_t j = table_probabilist[k][1];
        if((gameMap[i][j].state & OBSTRUCTION_BITS) == CELL_FREE) {
            gameMap[i][j].state |= CELL_OCCUPED_RED;   //Shame modification as above
            break;
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

void init_game(void) {
    chSemObjectInit(&gamestate_sem);
    gameState = 0;
}

void get_difficulty(uint8_t *a) {
    chSemWait(&gamestate_sem);
    *a = gamestate & GAMESTATE_BITS;
    chSemSignal(&gamestate_sem);
}

uint8_t get_state(void) {

}