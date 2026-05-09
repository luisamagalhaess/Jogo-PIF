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

    // Define a posição inicial do tabuleiro na tela
    // Centraliza horizontalmente o grid de 9 colunas
    int x_inicial = (800 - 9 * 64) / 2;
    int y_inicial = 100; // Distância do topo da janela até o início do tabuleiro

    
    //loop
    while (!WindowShouldClose()) {

        Vector2 mouse = GetMousePosition(); // Converte a posição do mouse em pixels para coluna e linha do tabuleiro
        int col_mouse = (mouse.x - x_inicial) / 64;
        int lin_mouse = (mouse.y - y_inicial) / 64;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){ //seleciona uma célula
            j.cursor_linha = lin_mouse;
            j.cursor_coluna = col_mouse;
        }

        BeginDrawing();
            ClearBackground(BLACK);
            for (int l = 0; l < LINHAS; l++) {
                for (int c = 0; c < COLUNAS; c++) {
                    Color cor = DARKGRAY;
                    if (l == lin_mouse && c == col_mouse) { //muda a cor ao passar o mouse
                        cor = GRAY;
                    }
                    if (l == j.cursor_linha && c == j.cursor_coluna){ //muda a cor ao selecionar uma célula
                        cor = YELLOW;
                    }

                    DrawRectangle(x_inicial + c * 64, y_inicial + l * 64, 64, 64, cor);
                    DrawRectangleLines(x_inicial + c * 64, y_inicial + l * 64, 64, 64, WHITE);
                }
            }
            DrawRectangle(40, 440, 154, 100, DARKGRAY);
            DrawText("Gerador", 40 + 10, 440 + 10, 20, WHITE);
            DrawText("Custo: 10", 40 + 10, 440 + 35, 16, WHITE);

            DrawRectangle(40 + 150 + 40, 440, 154, 100, DARKGRAY);
            DrawText("Torreta", 40 + 150 + 40 + 10, 440 + 10, 16, WHITE);
            DrawText("custo: 100", 40 + 150 + 40 + 10, 440 + 35, 16, WHITE);

            DrawRectangle(230 + 150 + 40, 440, 154, 100, DARKGRAY);
            DrawText("Muro", 230 + 150 + 40 + 10, 440 + 10, 20, WHITE);
            DrawText("Custo: 10", 230 + 150 + 40 + 10, 440 + 35, 16, WHITE);

            DrawRectangle(420 + 150 + 40, 440, 154, 100, DARKGRAY);
            DrawText("Bomba", 420 + 150 + 40 + 10, 440 + 10, 20, WHITE);
            DrawText("Custo: 100", 420 + 150 + 40 + 10, 440 + 35, 16, WHITE);


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