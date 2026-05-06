#include "render.h"
#include <stdio.h>
#include <stdlib.h>

char obter_simbolo(Jogo *j, int linha, int coluna){
    Alien *atual = j->aliens[linha];
    while(atual != NULL){
        if(atual->coluna == coluna){
            switch (atual->tipo) {
                case ALIEN_INVASOR:
                    return 'i';
                case ALIEN_BLINDADO:
                    return 'B';
                case ALIEN_KAMIKAZE:
                    return 'K';
            }
        }else{
            atual = atual->next;
        }
    }

    Defesa *d = j->grid[linha][coluna].defesa;

    switch (d->tipo){
        case DEFESA_GERADOR:
            return 'G';
        case DEFESA_MURO:
            return 'M';
        case DEFESA_TORRETA:
            return 'T';
        case DEFESA_BOMBA:
            return 'B';
    }
    return '.';
}

void renderizar(Jogo *j){
    LIMPAR;
    printf("Energia: %d | Vidas: %d | Score: %d | Onda: %d\n", j->energia, j->vidas, j->score, j->onda_atual);
    for(int l = 0; l < LINHAS; l++){
        for(int c = 0; c < COLUNAS; c++){
            printf("%c ", obter_simbolo(j, l, c));
        }
        printf("\n");
    }
}