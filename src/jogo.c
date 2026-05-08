#include <stdlib.h>
#include "jogo.h"

void iniciar_jogo(Jogo* j){
    for (int l = 0; l < LINHAS; l++){
        for(int c = 0; c < COLUNAS; c++){
            j->grid[l][c].defesa = NULL;
        }
    }

    for(int l = 0; l < LINHAS; l++){
        j->aliens[l] = NULL;
    }

    j->energia = 50;
    j->vidas = 5;
    j->score = 0;
    j->onda_atual = 1;
    j->cursor_linha = 0;
    j->cursor_coluna = 0;
}

void destruir_jogo(Jogo* j){ //liberar a memória alocada
    for(int l = 0; l < LINHAS; l++){
        for(int c = 0; c < COLUNAS; c++){
            if(j->grid[l][c].defesa != NULL){
                free(j->grid[l][c].defesa);
                j->grid[l][c].defesa = NULL;
            }
        }
    }

    for (int l = 0; l < LINHAS; l++) {
        destruir_lista(&j->aliens[l]);
    }
}