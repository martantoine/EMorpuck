 #include "algorithm.h"

bool check_if_2_alligned(uint8_t * table[NB_SIDE_CELLS][NB_SIDE_CELLS])
{
    
}



 void place_easy(uint8_t * table[NB_SIDE_CELLS][NB_SIDE_CELLS]) 
 /*this function should be used after veryfying that table as some free space, if not the case endgame()*/
 {
    bool cell_placed = true;
    while(cell_placed==false)
    {

        srand(time(0));
        int i=rand()/3;       //assign 2 random coordinates between 0 and 3
        int j=rand()/3; 

        if(*table[i][j]==CELL_FREE)
        {
            *table[i][j]=CELL_OCCUPED_RED; 
            cell_placed=true;
        }
    }
    return;
 }

 