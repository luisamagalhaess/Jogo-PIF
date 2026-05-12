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
    double ultimo_turno = GetTime();
    double intervalo = 3.0; //1 segundo por turno
    Texture2D fundo = LoadTexture("assets/fundo.png");
    Texture2D alien_invasor = LoadTexture("assets/alien_invasor.png");
    Texture2D alien_blindado = LoadTexture("assets/alien_blindado.png");
    Texture2D alien_kamikaze = LoadTexture("assets/alien_kamikaze.png");

    //loop
    while (!WindowShouldClose()) {

        Vector2 mouse = GetMousePosition();
        int col_mouse = (mouse.x - x_inicial) / 64;
        int lin_mouse = (mouse.y - y_inicial) / 64;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            j.cursor_linha = lin_mouse;
            j.cursor_coluna = col_mouse;
        }

        // LÓGICA DO TURNO
        if (GetTime() - ultimo_turno >= intervalo) {
            geradores_produzem(&j);
            torretas_atacam(&j);
            mover_aliens(&j);
            spawnar_alien(&j);
            ultimo_turno = GetTime();
        }

        // CÁLCULO DA ANIMAÇÃO (Interpolação de 0.0 a 1.0)
        float tempo_passado = GetTime() - ultimo_turno;
        float progresso_turno = tempo_passado / intervalo;
        if (progresso_turno > 1.0f) progresso_turno = 1.0f; // Trava em 1.0 por segurança

        BeginDrawing();
            ClearBackground(BLACK);
            
            // Fundo
            DrawTexturePro(fundo, (Rectangle){0, 0, fundo.width, fundo.height}, (Rectangle){0, 0, 800, 600}, (Vector2){0, 0}, 0, WHITE);

            // 1. DESENHAR O GRID (Agora transparente)
            for (int l = 0; l < LINHAS; l++) {
                for (int c = 0; c < COLUNAS; c++) {
                    Color cor = BLANK; // Transparente por padrão!
                    
                    if (l == lin_mouse && c == col_mouse) { 
                        cor = Fade(WHITE, 0.2f); // Branco transparente
                    }
                    if (l == j.cursor_linha && c == j.cursor_coluna){ 
                        cor = Fade(YELLOW, 0.4f); // Amarelo transparente
                    }

                    if (cor.a > 0) { // Só desenha o fundo se tiver cor
                        DrawRectangle(x_inicial + c * 64, y_inicial + l * 64, 64, 64, cor);
                    }
                    
                    // Bordas do grid bem suaves para não atrapalhar a arte
                    DrawRectangleLines(x_inicial + c * 64, y_inicial + l * 64, 64, 64, Fade(WHITE, 0.15f));
                }
            }

            // 2. DESENHAR OS ALIENS (Com movimento fluido)
            for (int l = 0; l < LINHAS; l++) {
                Alien *a = j.aliens[l];
                while (a != NULL) {
                    Texture2D tex;
                    if (a->tipo == ALIEN_INVASOR) tex = alien_invasor;
                    else if (a->tipo == ALIEN_BLINDADO) tex = alien_blindado;
                    else tex = alien_kamikaze;

                    // O truque: A posição visual começa uma coluna para trás e vai deslizando até a coluna atual
                    float coluna_visual = (a->coluna + 1) - progresso_turno;

                    DrawTexturePro(
                        tex,
                        (Rectangle){0, 0, tex.width, tex.height},
                        (Rectangle){x_inicial + coluna_visual * 64, y_inicial + l * 64, 64, 64},
                        (Vector2){0, 0},
                        0,
                        RAYWHITE
                    );
                    a = a->next;
                }
            }

            // 3. DESENHAR O MENU INFERIOR
            DrawRectangle(40, 440, 154, 100, Fade(DARKGRAY, 0.8f)); // Fundo do menu um pouco transparente também
            DrawText("Gerador", 40 + 10, 440 + 10, 20, WHITE); 
            DrawText("Custo: 10", 40 + 10, 440 + 35, 16, WHITE);

            DrawRectangle(40 + 150 + 40, 440, 154, 100, Fade(DARKGRAY, 0.8f));
            DrawText("Torreta", 40 + 150 + 40 + 10, 440 + 10, 16, WHITE);
            DrawText("Custo: 100", 40 + 150 + 40 + 10, 440 + 35, 16, WHITE);

            DrawRectangle(230 + 150 + 40, 440, 154, 100, Fade(DARKGRAY, 0.8f));
            DrawText("Muro", 230 + 150 + 40 + 10, 440 + 10, 20, WHITE);
            DrawText("Custo: 10", 230 + 150 + 40 + 10, 440 + 35, 16, WHITE);

            DrawRectangle(420 + 150 + 40, 440, 154, 100, Fade(DARKGRAY, 0.8f));
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