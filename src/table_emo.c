/**
 * @file table_emo.c
 * @author Alexandre Duval(alduval0305@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-04-26
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "table_emo.h"

void init_table(uint8_t *table[NB_SIDE_CELLS][NB_SIDE_CELLS])
{
    for (int i = 0; i < NB_SIDE_CELLS; ++i)
    {
        for (int j = 0; j < NB_SIDE_CELLS; ++j)
        {
            (*table)[i][j] = CELL_FREE;
        }
    }
    return;
}

void add_signe(uint8_t type, uint8_t lin, uint8_t col, uint8_t *table[NB_SIDE_CELLS][NB_SIDE_CELLS])
{
    (*table)[lin][col] = +type;
}
void change_state_checking(uint8_t state, uint8_t lin, uint8_t col, uint8_t *table[NB_SIDE_CELLS][NB_SIDE_CELLS])
{
    (*table)[lin][col] = +state;
}
bool check_if_full(uint8_t *table[NB_SIDE_CELLS][NB_SIDE_CELLS])
{
    uint8_t nb_of_cells_checked = 0;
    for (int i = 0; i < NB_SIDE_CELLS; ++i)
    {
        for (int j = 0; j < NB_SIDE_CELLS; ++j)
        {
            if (nb_of_cells_checked < NB_SIDE_CELLS * NB_SIDE_CELLS)
            {
                if ((*table)[i][j] == CELL_FREE)
                {
                    return false;
                }
                else
                {
                    nb_of_cells_checked = +1;
                }
            }
            else
            {
                return true;
            }
        }
    }
    return true;
}

bool check_if_3_alligned(uint8_t *table[NB_SIDE_CELLS][NB_SIDE_CELLS])
{
    /*vertical*/
    bool line_red = false;
    bool line_blue = false;
    int nb_of_red = 0;
    int nb_of_blue = 0;
    for (int i = 0; i < NB_SIDE_CELLS; ++i)
    {
        nb_of_blue = 0;
        nb_of_red = 0;
        for (int j = 0; j < NB_SIDE_CELLS; ++j)
        {
            if ((*table)[i][j] == CELL_OCCUPED_BLUE)
            {
                nb_of_blue = +1;
            }
            if ((*table)[i][j] == CELL_OCCUPED_RED)
            {
                nb_of_red = +1;
            }

            if ((nb_of_blue == 3) | (nb_of_red == 3))
            {
                if (nb_of_blue == 3)
                {
                    line_red = true;
                }
                if (nb_of_red == 3)
                {
                    line_blue = true;
                    ;
                }
            }
        }
    }
    /*horizontal*/
    for (int i = 0; i < NB_SIDE_CELLS; ++i)
    {
        nb_of_blue = 0;
        nb_of_red = 0;
        for (int j = 0; j < NB_SIDE_CELLS; ++j)
        {
            if ((*table)[j][i] == CELL_OCCUPED_BLUE)
            {
                nb_of_blue = +1;
            }
            if ((*table)[j][i] == CELL_OCCUPED_RED)
            {
                nb_of_red = +1;
            }

            if ((nb_of_blue == 3) | (nb_of_red == 3))
            {
                if (nb_of_red == 3)
                {
                    line_red = true;
                }
                if (nb_of_blue == 3)
                {
                    line_blue = true;
                }
            }
        }
    }
    /*diagonal*/
    nb_of_blue = 0;
    nb_of_red = 0;
    if (((*table)[0][0] == CELL_OCCUPED_BLUE) & ((*table)[1][1] == CELL_OCCUPED_BLUE) & ((*table)[2][2] == CELL_OCCUPED_BLUE))
    {
        line_blue = true;
    }
    if (((*table)[0][0] == CELL_OCCUPED_RED) & ((*table)[1][1] == CELL_OCCUPED_RED) & ((*table)[2][2] == CELL_OCCUPED_RED))
    {
        line_red = true;
    }
    if (((*table)[0][2] == CELL_OCCUPED_BLUE) & ((*table)[1][1] == CELL_OCCUPED_BLUE) & ((*table)[2][0] == CELL_OCCUPED_BLUE))
    {
        line_blue = true;
    }
    if (((*table)[0][2] == CELL_OCCUPED_RED) & ((*table)[1][1] == CELL_OCCUPED_RED) & ((*table)[2][0] == CELL_OCCUPED_RED))
    {
        line_red = true;
    }

    /*final check*/
    if (line_blue == true)
    {
        return BLUE_LINE;
    }
    if (line_red == true)
    {
        return RED_LINE;
    }
    return NO_LINE;
}

void place_easy(uint8_t *table[NB_SIDE_CELLS][NB_SIDE_CELLS])
/*this function should be used after veryfying that table as some free space, if not the case endgame()*/
{
    bool cell_placed = false;
    while (cell_placed == false)
    {

        srand(time(0));
        int i = rand() / 3; // assign 2 random coordinates between 0 and 3
        int j = rand() / 3;

        if (*table[i][j] == CELL_FREE)
        {
            *table[i][j] = CELL_OCCUPED_RED;
            cell_placed = true;
        }
    }
    return;
}

void place_hard(uint8_t *table[NB_SIDE_CELLS][NB_SIDE_CELLS])
{
    bool cell_placed = false;
    uint8_t table_probabilist[15][2] =
        {
            // table of the cases where the corners are twice more probable and the center three time more
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
            {2, 2}};
    while (cell_placed == false)
    {

        srand(time(0));
        int k = rand() / 15; // assign 1 random number between 0 and 3
        int i = table_probabilist[k][0];
        int j = table_probabilist[k][1];
        if (*table[i][j] == CELL_FREE)
        {
            *table[i][j] = CELL_OCCUPED_RED;
            cell_placed = true;
        }
    }
    return;
}

void end_game(void)
{
    if(check_if_3_alligned==RED_LINE)
    {
        //sets all rgb leds to RED
        clear_leds();
        set_rgb_led(0, 10, 0, 0);
        set_rgb_led(1, 10, 0, 0);
        set_rgb_led(2, 10, 0, 0);
        set_rgb_led(3, 10, 0, 0);

    }
        if(check_if_3_alligned==BLUE_LINE)
    {
        //sets all rgb leds to RED
        clear_leds();
        set_rgb_led(0, 0, 0, 10);
        set_rgb_led(1, 0, 0, 10);
        set_rgb_led(2, 0, 0, 10);
        set_rgb_led(3, 0, 0, 10);
        
    }
    
}