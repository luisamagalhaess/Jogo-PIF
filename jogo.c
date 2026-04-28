#include <stdlib.h>
#include "jogo.h"

void iniciar_jogo(Jogo* j){
    for (int l = 0; l < LINHAS; l++){
        for(int c = 0; c < COLUNAS; c++){
            j->grid[l][c].defesa = NULL;
        }
    }
}