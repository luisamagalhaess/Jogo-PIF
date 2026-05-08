#include <stdio.h>
#include <string.h>
#include "jogo.h"
#include "scores.h"
#include "raylib.h"

int main() {
    // 1. Declarar variáveis
    Jogo j;
    Score scores[100];
    int scores_salvos = 0;
    char nome[50];

    // 2. Inicializar
    InitWindow(800, 600, "Erid");
    iniciar_jogo(&j);
    carregar_scores(scores, &scores_salvos);

    int x_inicial = (800 - 9 * 64) / 2;
    int y_inicial = 100;

    
    //loop
    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();
        int col_mouse = (mouse.x - x_inicial) / 64;
        int lin_mouse = (mouse.y - y_inicial) / 64;
        BeginDrawing();
            ClearBackground(BLACK);
            for (int l = 0; l < LINHAS; l++) {
                for (int c = 0; c < COLUNAS; c++) {
                    Color cor = DARKGRAY;
                    if (l == lin_mouse && c == col_mouse) {
                            cor = GRAY;

}

                    DrawRectangle(x_inicial + c * 64, y_inicial + l * 64, 64, 64, cor);
                    DrawRectangleLines(x_inicial + c * 64, y_inicial + l * 64, 64, 64, WHITE);
                }
            }
        EndDrawing();
    }
    
    
    // 5. Game over
    Score novo_score;
    strcpy(novo_score.nome, nome);
    novo_score.vidas = j.vidas;
    salvar_score(novo_score);
    exibir_scores(scores, scores_salvos);

    // 6. Liberar memória
    CloseWindow();
    destruir_jogo(&j);
    return 0;
}