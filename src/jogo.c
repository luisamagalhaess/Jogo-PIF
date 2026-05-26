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
    j->cursor_linha = -1;
    j->cursor_coluna = -1;
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

void geradores_produzem(Jogo *j) {
    for (int l = 0; l < LINHAS; l++) {
        for (int c = 0; c < COLUNAS; c++) {
            if (j->grid[l][c].defesa != NULL){
                if(j->grid[l][c].defesa->tipo == DEFESA_GERADOR){
                    j->energia += j->grid[l][c].defesa->energia_por_turno;
                }
            }
        }
    }
}

void mover_aliens(Jogo *j) {
    for (int l = 0; l < LINHAS; l++) {
        Alien *a = j->aliens[l];
        while (a != NULL) {
            Alien *proximo = a->next;
            a->coluna--;
            if(j->grid[l][a->coluna].defesa != NULL){
                j->grid[l][a->coluna].defesa->vida -= a->dano;
                if(j->grid[l][a->coluna].defesa->vida <= 0){
                    if(j->grid[l][a->coluna].defesa->tipo == DEFESA_BOMBA){ //verifica se a defesa é uma bomba
                       explodir_bomba(j, l, a->coluna); //explode
                    }
                    destruir_defesa(j->grid[l][a->coluna].defesa);
                    j->grid[l][a->coluna].defesa = NULL;
                }
                a->coluna++;
                a = proximo;
                continue;
            }
            if (a->coluna < 0) {
                j->vidas--;
                remover_alien(&j->aliens[l], a);
            }
            a = proximo;
        }
    }
}

void spawnar_alien(Jogo *j) {
    int linha = rand() % LINHAS; //cria um alien novo numa linha aleatória
    int tipo = rand() % 3;
    
    Alien *novo = criar_alien(tipo, linha);
    if (novo == NULL) return;

    novo->next = j->aliens[linha]; // o next do novo aponta para o primeiro da lista
    j->aliens[linha] = novo;       // o novo vira o primeiro da lista
}

void torretas_atacam(Jogo *j){
    for (int l = 0; l < LINHAS; l++){
        for(int c = 0; c < COLUNAS; c++){
            if(j->grid[l][c].defesa != NULL){
                if(j->grid[l][c].defesa->tipo == DEFESA_TORRETA){ //percorre o grid procurando torretas

                    Alien *alvo = NULL;
                    Alien *a = j->aliens[l];

                    while (a != NULL) {
                        if (a->coluna > c) { // alien está à direita da torreta
                            if (alvo == NULL || a->coluna < alvo->coluna) { 
                                alvo = a;
                            }
                        }
                        a = a->next;
                    }
                    if (alvo != NULL) {
                        alvo->vida -= j->grid[l][c].defesa->dano;
                        
                        if (alvo->vida <= 0) {
                            j->score += 10;
                            remover_alien(&j->aliens[l], alvo);
                        }
                    }
                }
            }
        }
    }

}

void explodir_bomba(Jogo *j, int linha, int coluna){
    for (int l = linha -1; l <= linha +1;  l++){ // começa com -1 para pegar a linha de cima, passa pela linha da bomba e termina na linha de baixo(+1)
        for (int c = coluna - 1; c <= coluna + 1; c++){
            if (l >= 0 && l<LINHAS && c >= 0 && c < COLUNAS){//verifica se está dentro do grid, evita l e c serem negativos
                Alien *a = j->aliens[l];
                while (a != NULL){
                    Alien *proximo = a->next;
                    if (a->coluna == c){
                        a->vida -= 200;
                        if(a->vida <= 0){
                            remover_alien(&j->aliens[l], a);
                            j->score += 10;
                        }
                    }
                    a = proximo;
                }
                if(j->grid[l][c].defesa != NULL){
                    j->grid[l][c].defesa->vida -= 200;
                    if(j->grid[l][c].defesa->vida <= 0){
                        destruir_defesa(j->grid[l][c].defesa);
                        j->grid[l][c].defesa = NULL;
                    }
                }
            }

        }
    }
}