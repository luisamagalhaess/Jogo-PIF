#include <stdio.h>
#include <string.h>
#include "jogo.h"
#include "scores.h"
#include "raylib.h"
#include <time.h>
#include <stdlib.h>

int main() {
    // 1. Declarar variáveis
    Jogo j;
    Score scores[100];
    int scores_salvos = 0;
    char nome[50];

    // 2. Inicializar
    InitWindow(800, 600, "Erid");
    iniciar_jogo(&j);
    srand(time(NULL));
    carregar_scores(scores, &scores_salvos);

    // Define a posição inicial do tabuleiro na tela
    // Centraliza horizontalmente o grid de 9 colunas
    int x_inicial = (800 - 9 * 64) / 2;
    int y_inicial = 160; // Distância do topo da janela até o início do tabuleiro (Alterei de 100 -> 150 para enquadrar melhor o mapa com o fundo)
    double ultimo_turno = GetTime();
    double intervalo = 3.0; //1 segundo por turno
    Texture2D fundo = LoadTexture("assets/fundo.png");
    Texture2D alien_blindado = LoadTexture("assets/alien_blindado.png");
    Texture2D alien_kamikaze = LoadTexture("assets/alien_kamikaze.png");
    Texture2D gerador = LoadTexture("assets/gerador.png");
    Texture2D muro = LoadTexture("assets/muro.png");
    Texture2D torreta = LoadTexture("assets/torreta.png");
    Texture2D bomba = LoadTexture("assets/bomba.png");
    Texture2D alien_invasor = LoadTexture("assets/alien_invasor.png");


    int defesa_selecionada = -1;
    int y_menu = 500; 
    while (!WindowShouldClose()) {
        if(j.vidas > 0){
            Vector2 mouse = GetMousePosition();
            int col_mouse = (mouse.x - x_inicial) / 64;
            int lin_mouse = (mouse.y - y_inicial) / 64;

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                //verificar se o clique aconteceu nas cartas:
                if(mouse.y >= y_menu){
                    if(mouse.x >= 40 && mouse.x <= 40 + 154){
                        defesa_selecionada = DEFESA_GERADOR;
                    }else if(mouse.x >= 230 && mouse.x <= 230 + 154){
                        defesa_selecionada = DEFESA_TORRETA;
                    }else if(mouse.x >= 420 && mouse.x <= 420 + 154){
                        defesa_selecionada = DEFESA_MURO;
                    }else if(mouse.x >= 610 && mouse.x <= 610 + 154){
                        defesa_selecionada = DEFESA_BOMBA;
                    }
                }else{
                    j.cursor_linha = lin_mouse;
                    j.cursor_coluna = col_mouse;
                    if (defesa_selecionada != -1){
                        int custo = 0;
                        if(defesa_selecionada == DEFESA_GERADOR){
                            custo = 10;
                        }else if(defesa_selecionada == DEFESA_TORRETA){
                            custo = 100;
                        }else if(defesa_selecionada == DEFESA_MURO){
                            custo = 10;
                        }else if(defesa_selecionada == DEFESA_BOMBA){
                            custo = 100;
                        }

                        if(j.energia >= custo){
                            //caso já tenha uma defesa no local selecionado
                            if(j.grid[j.cursor_linha][j.cursor_coluna].defesa != NULL){
                                destruir_defesa(j.grid[j.cursor_linha][j.cursor_coluna].defesa);
                                j.grid[j.cursor_linha][j.cursor_coluna].defesa = NULL;
                            }
                            j.grid[j.cursor_linha][j.cursor_coluna].defesa = criar_defesa(defesa_selecionada);
                            j.energia -= custo;
                            defesa_selecionada = -1;
                        }
                    }
                }
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

                        float coluna_visual = (a->coluna + 1) - progresso_turno;
                        int frame_largura = tex.width / 4;
                        int frame_atual = (int)(GetTime() * 6) % 4;

                        DrawTexturePro(
                            tex,
                            (Rectangle){frame_atual * frame_largura, 0, frame_largura, tex.height},
                            (Rectangle){x_inicial + coluna_visual * 64, y_inicial + l * 64, 64, 64},
                            (Vector2){0, 0},
                            0,
                            RAYWHITE
                        );
                        a = a->next;
                    }
                }

                //desenha defesa
                for(int l = 0; l < LINHAS; l++){
                    for(int c = 0; c < COLUNAS; c++){
                        if(j.grid[l][c].defesa != NULL){
                            Texture2D tex;
                            switch (j.grid[l][c].defesa->tipo){
                                case DEFESA_GERADOR:
                                    tex = gerador;
                                    break;
                                case DEFESA_BOMBA:
                                    tex = bomba;
                                    break;
                                case DEFESA_MURO:
                                    tex = muro;
                                    break;
                                case DEFESA_TORRETA:
                                    tex = torreta;
                                    break;
                            }
                                int num_frames_defesa = 4;
                                int frame_largura_defesa = tex.width / num_frames_defesa;
                                int frame_atual_defesa = (int)(GetTime() * 6) % num_frames_defesa;

                                DrawTexturePro(
                                    tex,
                                    (Rectangle){frame_atual_defesa * frame_largura_defesa, 0, frame_largura_defesa, tex.height},
                                    (Rectangle){x_inicial + c * 64, y_inicial + l * 64, 64, 64},
                                    (Vector2){0, 0},
                                    0,
                                    RAYWHITE
                                );
                                                    }
                    }
                }

                // 3. DESENHAR O MENU INFERIOR

                // CARTA 1: GERADOR
                DrawRectangle(40, y_menu, 154, 90, Fade(DARKGRAY, 0.8f));
                DrawText("Gerador", 40 + 10, y_menu + 10, 20, WHITE); 
                DrawText("Custo: 10", 40 + 10, y_menu + 35, 16, WHITE);

                // CARTA 2: TORRETA
                DrawRectangle(40 + 150 + 40, y_menu, 154, 90, Fade(DARKGRAY, 0.8f));
                DrawText("Torreta", 40 + 150 + 40 + 10, y_menu + 10, 16, WHITE);
                DrawText("Custo: 100", 40 + 150 + 40 + 10, y_menu + 35, 16, WHITE);

                // CARTA 3: MURO (Como estava antes x Como ficou)
                DrawRectangle(230 + 150 + 40, y_menu, 154, 90, Fade(DARKGRAY, 0.8f));
                DrawText("Muro", 230 + 150 + 40 + 10, y_menu + 10, 20, WHITE);
                DrawText("Custo: 10", 230 + 150 + 40 + 10, y_menu + 35, 16, WHITE);

                // CARTA 4: BOMBA (Como estava antes x Como ficou)
                DrawRectangle(420 + 150 + 40, y_menu, 154, 90, Fade(DARKGRAY, 0.8f));
                DrawText("Bomba", 420 + 150 + 40 + 10, y_menu + 10, 20, WHITE);
                DrawText("Custo: 100", 420 + 150 + 40 + 10, y_menu + 35, 16, WHITE);

                //exibir energia, vidas, score e ondas (x, y, tamanho, cor)
                DrawText(TextFormat("Energia: %d", j.energia), 10, 15, 20, WHITE);
                DrawText(TextFormat("Vidas: %d", j.vidas), 200, 15, 20, WHITE);
                DrawText(TextFormat("Score: %d", j.score), 400, 15, 20, WHITE);
                DrawText(TextFormat("Onda atual: %d", j.onda_atual), 600, 15, 20, WHITE);

            EndDrawing();
        }else{
            BeginDrawing();
                ClearBackground(BLACK);
                DrawTexturePro(fundo, (Rectangle){0, 0, fundo.width, fundo.height}, (Rectangle){0, 0, 800, 600}, (Vector2){0, 0}, 0, WHITE);
                DrawText("GAME OVER", 250, 200, 60, WHITE);
                DrawText(TextFormat("Score: %d", j.score), 300, 290, 30, WHITE);
                DrawText("Clique para jogar novamente", 230, 370, 20, WHITE);
                
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    destruir_jogo(&j);
                    iniciar_jogo(&j);
                    ultimo_turno = GetTime();
                    defesa_selecionada = -1;
                }
            EndDrawing();
        }
    }
    
    
    // 5. Game over
    Score novo_score;
    strcpy(novo_score.nome, nome);
    novo_score.vidas = j.vidas;
    salvar_score(novo_score);
    exibir_scores(scores, scores_salvos);

    // 6. Liberar memória
    UnloadTexture(fundo);
    UnloadTexture(alien_invasor);
    UnloadTexture(alien_blindado);
    UnloadTexture(alien_kamikaze);
    UnloadTexture(gerador);
    UnloadTexture(muro);
    UnloadTexture(torreta);
    UnloadTexture(bomba);
    CloseWindow();
    destruir_jogo(&j);
    return 0;
}