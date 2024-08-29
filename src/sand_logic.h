#ifndef SAND_LOGIC__H
#define SAND_LOGIC__H
#include "structs_and_definitions.h"
int coinflip=0;

#define AIR 0;
#define SAND 1;
#define INERT_GAS 2;
#define WATER 3;

void regular_sand(char sim[][WIDTH], char aux_sim[][WIDTH], int i, int j){       
        if(j == WIDTH - 1) return;

        // Columna izquierda
        if(i==0){            
            if(sim[i][j+1] == 0){
                aux_sim[i][j] = 0; aux_sim[i][j+1] = sim[i][j];}
            else if(sim[i+1][j+1] == 0){
                aux_sim[i][j] = 0; aux_sim[i+1][j+1] = sim[i][j];}
            else if(sim[i][j+1]==3){
                aux_sim[i][j+1]=sim[i][j];
                aux_sim[i][j]=3;
            }
        }

        // Columna derecha del todo
        else if(i>WIDTH-2){
            if(sim[i][j+1] == 0){
                aux_sim[i][j] = 0;
                aux_sim[i][j+1] = sim[i][j];
            }
            else if(sim[i-1][j+1] == 0){
                aux_sim[i][j] = 0;
                aux_sim[i-1][j+1] = sim[i][j];
            }
            else if(sim[i][j+1]==3){
                aux_sim[i][j+1]=sim[i][j];
                aux_sim[i][j]=3;
            }
        }

        // En medio
        else{
            
            if(sim[i][j+1] == 0){
                aux_sim[i][j] = 0;
                aux_sim[i][j+1] = sim[i][j];
            }

            else if(sim[i+1][j+1] == 0 && sim[i-1][j+1] == 0){
                coinflip=rand()%2;
                aux_sim[i][j]=0;
                if(coinflip==0) aux_sim[i+1][j+1]= sim[i][j];
                else aux_sim[i+1][j+1]= sim[i][j];
                
            }
            else if(sim[i+1][j+1] == 0){
                aux_sim[i][j] = 0;
                aux_sim[i+1][j+1] = sim[i][j];
            }
            else if(sim[i-1][j+1] == 0){
                aux_sim[i][j] = 0;
                aux_sim[i-1][j+1] = sim[i][j];
            }
            else if(sim[i][j+1]==3){
                aux_sim[i][j+1]=sim[i][j];
                aux_sim[i][j]=3;
            }

        }
}

void inert_gas(char sim[][WIDTH], char aux_sim[][WIDTH], int i, int j){
        if(j==0){
            return;
        }
        // Columna izquierda del todo
        if(i==0){
            if(sim[i][j-1] == 0){
                aux_sim[i][j] = 0;
                aux_sim[i][j-1] = sim[i][j];
            }
            else if(sim[i+1][j-1] == 0){
                aux_sim[i][j] = 0;
                aux_sim[i+1][j-1] = sim[i][j];
            }
        }

        // Columna derecha del todo
        else if(i>WIDTH-2){
            if(sim[i][j-1] == 0){
                aux_sim[i][j] = 0;
                aux_sim[i][j-1] = sim[i][j];
            }
            else if(sim[i-1][j-1] == 0){
                aux_sim[i][j] = 0;
                aux_sim[i-1][j-1] = sim[i][j];
            }
        }

        // Medio
        else{
            if(sim[i][j-1] == 0){
                aux_sim[i][j] = 0;
                aux_sim[i][j-1] = sim[i][j];
            }

            else if(sim[i+1][j-1] == 0 && sim[i-1][j-1] == 0){
                coinflip=rand()%2;
                aux_sim[i][j]=0;
                if(coinflip==0) aux_sim[i+1][j-1]= sim[i][j];
                else aux_sim[i-1][j-1]= sim[i][j];
                
            }

            else if(sim[i+1][j-1] == 0){
                aux_sim[i][j] = 0;
                aux_sim[i+1][j-1] = sim[i][j];
            }
            else if(sim[i-1][j-1] == 0){
                aux_sim[i][j] = 0;
                aux_sim[i-1][j-1] = sim[i][j];
            }

        }
}


// This is where I'd put my accurate water cellular automaton, IF I COULD MAKE ONE
void basic_water(char sim[][WIDTH], char aux_sim[][WIDTH], int i, int j) {
    if(j>=WIDTH - 1) return;

   // Left column, except bottom left corner
   if(i==0 && j!=WIDTH){
        // Down
        if(sim[i][j+1]==0){aux_sim[i][j]=0; aux_sim[i][j+1]=sim[i][j];}
        // Down right
        else if(sim[i+1][j+1]==0){aux_sim[i][j]=0; aux_sim[i+1][j+1]=sim[i][j];}
        // Right
        else if(sim[i+1][j]==0){aux_sim[i][j]=0; aux_sim[i+1][j]=sim[i][j];}
   }

   // Bottom left corner
   else if(i==0 && j==WIDTH){
        // Right
        if(sim[i+1][j]==0){aux_sim[i][j]=0; aux_sim[i+1][j]=sim[i][j];}
   }

   // Right column, except bottom left corner
   else if(i>WIDTH-2 && j!=WIDTH){
        // Down
        if(sim[i][j+1]==0){aux_sim[i][j]=0; aux_sim[i][j+1]=sim[i][j];}
        // Down left
        else if(sim[i-1][j+1]==0){aux_sim[i][j]=0; aux_sim[i-1][j+1]=sim[i][j];}
        // Left
        else if(sim[i-1][j]==0){aux_sim[i][j]=0; aux_sim[i-1][j]=sim[i][j];}
   }

   // Bottom right corner
   else if(i>WIDTH-2 && j==WIDTH){
        // Left
        if(sim[i-1][j]==0){aux_sim[i][j]=0; aux_sim[i-1][j]=sim[i][j];}
   }

   else{
        // Down
        if(sim[i][j+1]==0){aux_sim[i][j]=0; aux_sim[i][j+1]=sim[i][j];}

        else if(sim[i+1][j+1] == 0 && sim[i-1][j+1] == 0){
            coinflip=rand()%2;
            aux_sim[i][j]=0;
            if(coinflip==0) aux_sim[i+1][j+1]= sim[i][j];
            else aux_sim[i+1][j+1]= sim[i][j];
        }
        // Down left
        else if(sim[i-1][j+1]==0){aux_sim[i][j]=0; aux_sim[i-1][j+1]=sim[i][j];}
        // Down right
        else if(sim[i+1][j+1]==0){aux_sim[i][j]=0; aux_sim[i+1][j+1]=sim[i][j];}
        // Left
        else if(sim[i-1][j]==0){aux_sim[i][j]=0; aux_sim[i-1][j]=sim[i][j];}
   }
}






#endif // SAND_LOGIC__H