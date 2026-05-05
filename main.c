#include <stdio.h>
#include <string.h>
#include "jogo.h"
#include "scores.h"
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

int main() {
    // 1. Declarar variáveis
    Jogo j;
    Score scores[100];
    int scores_salvos = 0;
    char nome[50];

    // 2. Inicializar
    screenInit(1);
    keyboardInit();
    iniciar_jogo(&j);
    carregar_scores(scores, &scores_salvos);

    // 3. Pedir nome do jogador
    keyboardDestroy(); // desativa modo especial do teclado temporariamente
    screenGotoxy(10, 5);
    printf("Digite seu nome: ");
    fgets(nome, 50, stdin);
    nome[strcspn(nome, "\n")] = '\0'; // remove o \n do final
    keyboardInit(); // reativa

    // 4. Loop do jogo
    while (j.vidas > 0) {
        if (keyhit()) {
            int tecla = readch();
        
            if (tecla == 'w') {
                if (j.cursor_linha > 0){ //só move se não estiver na primeira linha
                    j.cursor_linha--;
                }
            }

            if (tecla == 's') {
                if (j.cursor_linha < LINHAS - 1){ // se não chegou na última linha
                    j.cursor_linha++;
                }
            }

            if (tecla == 'a'){
                if (j.cursor_coluna > 0){ //só move se não estiver na primeira coluna
                    j.cursor_coluna--;
                }
            }

            if (tecla == 'd'){
                if (j.cursor_coluna < COLUNAS - 1){ // se não chegou na última coluna
                    j.cursor_coluna++;
                }
            }

            if (tecla == '1'){
                if (j.energia >= 10){
                    Defesa *d = criar_defesa(DEFESA_GERADOR); // cria e guarda
                    j.grid[j.cursor_linha][j.cursor_coluna].defesa = d; // coloca no grid
                    j.energia = j.energia - 10; // desconta energia
                }
            }

            if (tecla == '2'){
                if (j.energia >= 100){
                    Defesa *d = criar_defesa(DEFESA_TORRETA); // cria e guarda
                    j.grid[j.cursor_linha][j.cursor_coluna].defesa = d; // coloca no grid
                    j.energia = j.energia - 100; // desconta energia
                }
            }

            if (tecla == '3'){
                if (j.energia >= 10){
                    Defesa *d = criar_defesa(DEFESA_MURO); // cria e guarda
                    j.grid[j.cursor_linha][j.cursor_coluna].defesa = d; // coloca no grid
                    j.energia = j.energia - 10; // desconta energia
                }
            }

            if (tecla == '4'){
                if (j.energia >= 100){
                    Defesa *d = criar_defesa(DEFESA_BOMBA); // cria e guarda
                    j.grid[j.cursor_linha][j.cursor_coluna].defesa = d; // coloca no grid
                    j.energia = j.energia - 100; // desconta energia
                }
            }

            if (tecla == 'r'){
                if (j.grid[j.cursor_linha][j.cursor_coluna].defesa != NULL){ //checa se tem alguma defesa na célula
                    destruir_defesa(j.grid[j.cursor_linha][j.cursor_coluna].defesa); //libera a memória
                    j.grid[j.cursor_linha][j.cursor_coluna].defesa = NULL; //celula fica vazia
                } 
            }

            if (tecla == 'q') {
                break;
            }
        }




// o jogo continua independente de ter tecla ou não
    }
    

    // 5. Game over
    Score novo_score;
    strcpy(novo_score.nome, nome);
    novo_score.vidas = j.vidas;
    salvar_score(novo_score);
    exibir_scores(scores, scores_salvos);

    // 6. Liberar memória
    keyboardDestroy();
    screenDestroy();
    destruir_jogo(&j);
    return 0;
}