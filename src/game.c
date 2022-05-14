#include "game.h"
#include "defs.h"
#include <stdlib.h>
#include <ch.h>
#include <leds.h>

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
    //for(uint8_t x = GAMEMAP_CENTER-1; x <= GAMEMAP_CENTER+1; x++)
    //    for(uint8_t y = GAMEMAP_CENTER-1; y <= GAMEMAP_CENTER+1; y++)
    //        gameMap[x][y].state = CELL_OCCUPED;

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

uint8_t search_winner(cell_t gameMap[SIDE_NCELL][SIDE_NCELL]) {
    int nb_of_red = 0;
    int nb_of_blue = 0;

    for (int x = -1 + GAMEMAP_CENTER; x < GAMEMAP_CENTER + 3; x++) {
        /*vertical*/
        nb_of_blue = 0;
        nb_of_red = 0;
        for (int y = -1 + GAMEMAP_CENTER; y < GAMEMAP_CENTER + 3; y++) {
            if ((gameMap[x][y].state & OBSTRUCTION_BITS) == CELL_OCCUPED_BLUE)
                nb_of_blue++;
            else if((gameMap[x][y].state & OBSTRUCTION_BITS) == CELL_OCCUPED_RED)
                nb_of_red++;

            if (nb_of_red == 3)
                return RED;
            else if (nb_of_blue == 3)
                return BLUE;
        }

        /*horizontal*/
        nb_of_blue = 0;
        nb_of_red = 0;
        for (int y = -1 + GAMEMAP_CENTER; y < GAMEMAP_CENTER + 3; y++) {
            if ((gameMap[y][x].state & OBSTRUCTION_BITS) == CELL_OCCUPED_BLUE)
                nb_of_blue++;
            else if((gameMap[y][x].state & OBSTRUCTION_BITS) == CELL_OCCUPED_RED)
                nb_of_red++;

            if (nb_of_red == 3)
                return RED;
            else if (nb_of_blue == 3)
                return BLUE;
        }
    }

    /*diagonal*/
    if (((gameMap[0][0].state & OBSTRUCTION_BITS) == CELL_OCCUPED_BLUE) &&
        ((gameMap[1][1].state & OBSTRUCTION_BITS) == CELL_OCCUPED_BLUE) &&
        ((gameMap[2][2].state & OBSTRUCTION_BITS) == CELL_OCCUPED_BLUE))
    {
        return BLUE;
    }
    else if (((gameMap[0][0].state & OBSTRUCTION_BITS) == CELL_OCCUPED_RED) &&
             ((gameMap[1][1].state & OBSTRUCTION_BITS) == CELL_OCCUPED_RED) &&
             ((gameMap[2][2].state & OBSTRUCTION_BITS) == CELL_OCCUPED_RED))
    {
        return RED;
    }
    else if (((gameMap[0][2].state & OBSTRUCTION_BITS) == CELL_OCCUPED_BLUE) &&
             ((gameMap[1][1].state & OBSTRUCTION_BITS) == CELL_OCCUPED_BLUE) &&
             ((gameMap[2][0].state & OBSTRUCTION_BITS) == CELL_OCCUPED_BLUE))
    {
        return BLUE;
    }
    else if (((gameMap[0][2].state & OBSTRUCTION_BITS) == CELL_OCCUPED_RED) &&
             ((gameMap[1][1].state & OBSTRUCTION_BITS) == CELL_OCCUPED_RED) &&
             ((gameMap[2][0].state & OBSTRUCTION_BITS) == CELL_OCCUPED_RED))
    {
        return RED;
    }

    bool full = true;
    for (int x = -1; x < 3; x++)
        for (int y = -1; y < 3; y++)
            if ((gameMap[x + GAMEMAP_CENTER][y + GAMEMAP_CENTER].state & OBSTRUCTION_BITS) == CELL_FREE)
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
            .x = GAMEMAP_CENTER - 3 + (rand() % 4),
            .y = GAMEMAP_CENTER - 3 + (rand() % 4)
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
        case RED:
            chSemWait(&gamestates_sem);
            gamestates = (gamestates & ~STATE_BITS) | STATE_END;
            gamestates = (gamestates & ~WINNER_BITS) | WINNER_RED;
            chSemSignal(&gamestates_sem);
            break;
        case BLUE:
            chSemWait(&gamestates_sem);
            gamestates = (gamestates & ~STATE_BITS) | STATE_END;
            gamestates = (gamestates & ~WINNER_BITS) | WINNER_BLUE;
            chSemSignal(&gamestates_sem);
            break;
        case DRAW:
            chSemWait(&gamestates_sem);
            gamestates = (gamestates & ~STATE_BITS) | STATE_END;
            gamestates = (gamestates & ~WINNER_BITS) | WINNER_DRAW;
            chSemSignal(&gamestates_sem);
            break;
        default : //case NONE:
            break;
    }
}

void toggle_rgb_leds(rgb_led_name_t led_number, uint8_t red_val, uint8_t green_val, uint8_t blue_val) {
    static int b = 0;
    if(b==0) {
        set_rgb_led(led_number, red_val, green_val, blue_val);
        b = 1;
    }
    else {
        set_rgb_led(led_number, 0, 0, 0);
        b = 0;
    }
}

void show_stateofgame(void) {
    
    if((gamestates & STATE_BITS) != STATE_END) {
        clear_leds();
        if((gamestates & DIFFICULTY_BITS) == DIFFICULTY_EASY) {
            set_rgb_led(LED8,0,0,0xFF);
            set_rgb_led(LED2,0,0,0xFF);
        }
        else if((gamestates & DIFFICULTY_BITS) == DIFFICULTY_HARD) {
            set_rgb_led(LED8,0xFF,0,0);
            set_rgb_led(LED2,0xFF,0,0);
        }
    }
    if((gamestates & STATE_BITS) == STATE_START) {
        set_rgb_led(LED4,0,0xFF,0);
        set_rgb_led(LED6,0,0xFF,0);
    }
    else if((gamestates & STATE_BITS) == STATE_PLAYING) {
        set_rgb_led(LED4,0xFF,0,0);
        set_rgb_led(LED6,0xFF,0,0);
        if((gamestates & SCANNING_BITS) == SCANNING_RED) {
            set_rgb_led(LED2,0xFF,0,0);
            set_rgb_led(LED4,0xFF,0,0);
            set_rgb_led(LED6,0xFF,0,0);
            set_rgb_led(LED8,0xFF,0,0);
        }
        else if((gamestates & SCANNING_BITS) == SCANNING_BLUE) {
            set_rgb_led(LED2,0,0,0xFF);
            set_rgb_led(LED4,0,0,0xFF);
            set_rgb_led(LED6,0,0,0xFF);
            set_rgb_led(LED8,0,0,0xFF);
        }
        else if((gamestates & SCANNING_BITS) == SCANNING_NONE) {
            set_rgb_led(LED2,0xFF,0xFF,0xFF);
            set_rgb_led(LED4,0xFF,0xFF,0xFF);
            set_rgb_led(LED6,0xFF,0xFF,0xFF);
            set_rgb_led(LED8,0xFF,0xFF,0xFF);
        }
    }
    else if((gamestates & STATE_BITS) == STATE_WAITING_PLAYER) {
        set_rgb_led(LED4,0,0,0xFF);
        set_rgb_led(LED6,0,0,0xFF);
    }
    else if((gamestates & STATE_BITS) == STATE_END) {
        if((gamestates & WINNER_BITS) == WINNER_RED) {
            toggle_rgb_leds(LED2,0xFF,0,0);
            toggle_rgb_leds(LED4,0xFF,0,0);
            toggle_rgb_leds(LED6,0xFF,0,0);
            toggle_rgb_leds(LED8,0xFF,0,0);
        }
        else if((gamestates & WINNER_BITS) == WINNER_BLUE) {
            toggle_rgb_leds(LED2,0,0,0xFF);
            toggle_rgb_leds(LED4,0,0,0xFF);
            toggle_rgb_leds(LED6,0,0,0xFF);
            toggle_rgb_leds(LED8,0,0,0xFF);
        }
        else if((gamestates & WINNER_BITS) == WINNER_DRAW) {
            toggle_rgb_leds(LED2,0xFF,0xFF,0xFF);
            toggle_rgb_leds(LED4,0xFF,0xFF,0xFF);
            toggle_rgb_leds(LED6,0xFF,0xFF,0xFF);
            toggle_rgb_leds(LED8,0xFF,0xFF,0xFF);
        }
    }
}