#include "table.h"

#define CELL_PRIORITY 1

// redundant, I've already a more complete function in shared_var
void init_table(uint8_t **table) {
    for(int i = 0; i < GAMEMAP_SIDE_NCELL; i++) {
        for(int j = 0; j < GAMEMAP_SIDE_NCELL; j++) {
            table[i][j] = CELL_FREE;
}

void add_signe(uint8_t type, uint8_t lin, uint8_t col,uint8_t * table[GAMEMAP_SIDE_NCELL][GAMEMAP_SIDE_NCELL]) {
    (*table)[lin][col]=+type;
}
void change_state_checking(uint8_t state, uint8_t lin, uint8_t col,uint8_t * table[GAMEMAP_SIDE_NCELL][GAMEMAP_SIDE_NCELL])
{
      (*table)[lin][col]=+state;
}
void change_priority_placement(uint8_t state, uint8_t lin, uint8_t col, uint8_t * table[GAMEMAP_SIDE_NCELL][GAMEMAP_SIDE_NCELL])
{
    (*table)[lin][col]=+state;
}
bool check_if_full(uint8_t * table[GAMEMAP_SIDE_NCELL][GAMEMAP_SIDE_NCELL])
{
    uint8_t nb_of_cells_checked=0;
    for(int i = 0; i<GAMEMAP_SIDE_NCELL; i++)
        {
            for(int j = 0; j<GAMEMAP_SIDE_NCELL; j++)
            {
                if (nb_of_cells_checked<GAMEMAP_SIDE_NCELL*GAMEMAP_SIDE_NCELL)
                {
                    if((*table)[i][j]==CELL_FREE)
                    {
                        return false;
                    }
                    else
                    {
                        nb_of_cells_checked=+1;
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

bool check_if_2_alligned(uint8_t * table[GAMEMAP_SIDE_NCELL][GAMEMAP_SIDE_NCELL]) {
    /*vertical*/
    int nb_of_red = 0;
    int nb_of_blue = 0;

    for(int i = 0; i < GAMEMAP_SIDE_NCELL; i++)
    {
        nb_of_blue=0;
        nb_of_red=0;
        for(int j = 0; j < GAMEMAP_SIDE_NCELL; j++) {
        if((*table)[i][j] == CELL_OCCUPED_BLUE)
            nb_of_blue=+1;
        if((*table)[i][j] == CELL_OCCUPED_RED)
                nb_of_red=+1;
        if(nb_of_blue == 2)
            for(int k=0; k < GAMEMAP_SIDE_NCELL; ++k)
                if((*table)[i][k] == CELL_FREE)
                {
                    change_priority_placement(CELL_PRIORITY,i,k,*table);
                    return true;
                }
        if(nb_of_red == 2)
            for(int k=0; k< GAMEMAP_SIDE_NCELL; ++k)
                if((*table)[i][k]==CELL_FREE)
                {
                    change_priority_placement(CELL_PRIORITY,i,k,*table);
                    return true;
                }
    }

    /*horizontal*/
    nb_of_red=0;
    nb_of_blue=0;
    for(int i = 0; i<GAMEMAP_SIDE_NCELL; i++) {
        nb_of_blue = 0;
        nb_of_red = 0;
        for(int j = 0; j<GAMEMAP_SIDE_NCELL; j++) {
            if((*table)[j][i]==CELL_OCCUPED_BLUE)
                nb_of_blue=+1;
            if((*table)[j][i]==CELL_OCCUPED_RED)
                nb_of_red=+1;

            if(nb_of_blue=2)
                for(int k=0; k< GAMEMAP_SIDE_NCELL; ++k)
                    if((*table)[k][i]==CELL_FREE) {
                        change_priority_placement(CELL_PRIORITY,k,i,*table);
                        return true;
                    }
        }
        if(nb_of_red=2)
            for(int k=0; k< GAMEMAP_SIDE_NCELL; ++k)
                if((*table)[k][i]==CELL_FREE) {
                    change_priority_placement(CELL_PRIORITY,k,i,*table);
                    return true;
                }
    }

    /*diagonal*/
    // not missing somthing ?
}