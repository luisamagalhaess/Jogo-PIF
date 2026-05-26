#include <stdio.h>
#include <string.h>
#include "jogo.h"
#include "scores.h"
#include "raylib.h"
#include <time.h>
#include <stdlib.h>
#include <math.h> 

#define MAX_PROJETEIS 50

typedef struct {
    float x_origem, y_origem;
    float x_destino, y_destino;
    double tempo_inicio;
    float duracao;
    int dano;
    int linha;
    int ativo;
} Projetil;

void spawnar_projeteis(Jogo *j, Projetil projeteis[], int x_inicial, int y_inicial) {
    for (int l = 0; l < LINHAS; l++) {
        for (int c = 0; c < COLUNAS; c++) {
            if (j->grid[l][c].defesa == NULL) continue;
            if (j->grid[l][c].defesa->tipo != DEFESA_TORRETA) continue;

            Alien *alvo = NULL;
            Alien *a = j->aliens[l];
            while (a != NULL) {
                if (a->coluna > c) {
                    if (alvo == NULL || a->coluna < alvo->coluna) {
                        alvo = a;
                    }
                }
                a = a->next;
            }

            if (alvo == NULL) continue;

            for (int i = 0; i < MAX_PROJETEIS; i++) {
                if (!projeteis[i].ativo) {
                    projeteis[i].x_origem  = (float)(x_inicial + c * 64 + 32);
                    projeteis[i].y_origem  = (float)(y_inicial + l * 64 + 32);
                    projeteis[i].x_destino = (float)(x_inicial + alvo->coluna * 64 + 32);
                    projeteis[i].y_destino = (float)(y_inicial + l * 64 + 32);
                    projeteis[i].tempo_inicio = GetTime();
                    projeteis[i].duracao   = 0.8f;
                    projeteis[i].dano      = j->grid[l][c].defesa->dano;
                    projeteis[i].linha     = l;
                    projeteis[i].ativo     = 1;
                    break;
                }
            }
        }
    }
}

int main() {
    Jogo j;
    Score scores[100];
    int scores_salvos = 0;
    char nome[50];

    InitWindow(800, 600, "Erid");
    iniciar_jogo(&j);
    srand(time(NULL));
    carregar_scores(scores, &scores_salvos);

    int x_inicial = (800 - 9 * 64) / 2;
    int y_inicial = 160;
    double ultimo_turno = GetTime();
    double intervalo = 3.0;
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
    int estado = 0; // 0=história, 1=título, 2=jogo, 3=game over
    Projetil projeteis[MAX_PROJETEIS] = {0};


    while (!WindowShouldClose()) {

        // ESTADO 0: HISTÓRIA
        if (estado == 0) {
            BeginDrawing();
                ClearBackground(BLACK);
                DrawTexturePro(fundo, (Rectangle){0, 0, fundo.width, fundo.height}, (Rectangle){0, 0, 800, 600}, (Vector2){0, 0}, 0, Fade(WHITE, 0.3f));
                DrawText("O planeta Erid estava em paz...", 100, 150, 20, WHITE);
                DrawText("Ate que sinais desconhecidos", 100, 190, 20, WHITE);
                DrawText("comecaram a surgir no espaco.", 100, 230, 20, WHITE);
                DrawText("Forcas alienígenas se aproximam.", 100, 270, 20, WHITE);
                DrawText("Voce e o ultimo defensor de Erid.", 100, 310, 20, WHITE);
                DrawText("Clique para continuar...", 250, 400, 18, GRAY);
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    estado = 1;
                }
            EndDrawing();

        // ESTADO 1: TÍTULO
        } else if (estado == 1) {
            BeginDrawing();
                ClearBackground(BLACK);
                DrawTexturePro(fundo, (Rectangle){0, 0, fundo.width, fundo.height}, (Rectangle){0, 0, 800, 600}, (Vector2){0, 0}, 0, WHITE);
                DrawText("ERID", 280, 180, 80, WHITE);
                DrawText("Defesa do Planeta", 230, 270, 25, LIGHTGRAY);

                // Botão iniciar
                DrawRectangle(280, 360, 240, 60, Fade(DARKGRAY, 0.8f));
                DrawText("Iniciar Batalha", 305, 378, 22, WHITE);

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    Vector2 m = GetMousePosition();
                    if (m.x >= 280 && m.x <= 520 && m.y >= 360 && m.y <= 420) {
                        estado = 2;
                        ultimo_turno = GetTime();
                    }
                }
            EndDrawing();

        // ESTADO 2: JOGO
        } else if (estado == 2) {
            if (j.vidas <= 0) {
                estado = 3;
            }

            Vector2 mouse = GetMousePosition();
            int col_mouse = (mouse.x - x_inicial) / 64;
            int lin_mouse = (mouse.y - y_inicial) / 64;

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
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
                        if(defesa_selecionada == DEFESA_GERADOR) custo = 10;
                        else if(defesa_selecionada == DEFESA_TORRETA) custo = 100;
                        else if(defesa_selecionada == DEFESA_MURO) custo = 10;
                        else if(defesa_selecionada == DEFESA_BOMBA) custo = 100;

                        if(j.energia >= custo){
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

            if (GetTime() - ultimo_turno >= intervalo) {
                geradores_produzem(&j);
                spawnar_projeteis(&j, projeteis, x_inicial, y_inicial);
                mover_aliens(&j);
                spawnar_alien(&j);
                verificar_onda(&j);
                ultimo_turno = GetTime();
            }

            float progresso_turno = (GetTime() - ultimo_turno) / intervalo;
            if (progresso_turno > 1.0f) progresso_turno = 1.0f;

            BeginDrawing();
                ClearBackground(BLACK);
                DrawTexturePro(fundo, (Rectangle){0, 0, fundo.width, fundo.height}, (Rectangle){0, 0, 800, 600}, (Vector2){0, 0}, 0, WHITE);

                for (int l = 0; l < LINHAS; l++) {
                    for (int c = 0; c < COLUNAS; c++) {
                        Color cor = BLANK;
                        if (l == lin_mouse && c == col_mouse) cor = Fade(WHITE, 0.2f);
                        if (l == j.cursor_linha && c == j.cursor_coluna) cor = Fade(YELLOW, 0.4f);
                        if (cor.a > 0) DrawRectangle(x_inicial + c * 64, y_inicial + l * 64, 64, 64, cor);
                        DrawRectangleLines(x_inicial + c * 64, y_inicial + l * 64, 64, 64, Fade(WHITE, 0.15f));
                    }
                }

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

                        DrawTexturePro(tex,
                            (Rectangle){frame_atual * frame_largura, 0, frame_largura, tex.height},
                            (Rectangle){x_inicial + coluna_visual * 64, y_inicial + l * 64, 64, 64},
                            (Vector2){0, 0}, 0, RAYWHITE);
                        a = a->next;
                    }
                }

                for(int l = 0; l < LINHAS; l++){
                    for(int c = 0; c < COLUNAS; c++){
                        if(j.grid[l][c].defesa != NULL){
                            Texture2D tex;
                            switch (j.grid[l][c].defesa->tipo){
                                case DEFESA_GERADOR: tex = gerador; break;
                                case DEFESA_BOMBA:   tex = bomba;   break;
                                case DEFESA_MURO:    tex = muro;    break;
                                case DEFESA_TORRETA: tex = torreta; break;
                            }
                            int frame_largura_defesa = tex.width / 4;
                            int frame_atual_defesa = (int)(GetTime() * 6) % 4;
                            DrawTexturePro(tex,
                                (Rectangle){frame_atual_defesa * frame_largura_defesa, 0, frame_largura_defesa, tex.height},
                                (Rectangle){x_inicial + c * 64, y_inicial + l * 64, 64, 64},
                                (Vector2){0, 0}, 0, RAYWHITE);
                        }
                    }
                }

                DrawRectangle(40, y_menu, 154, 90, Fade(DARKGRAY, 0.8f));
                DrawText("Gerador", 40 + 10, y_menu + 10, 20, WHITE);
                DrawText("Custo: 10", 40 + 10, y_menu + 35, 16, WHITE);

                DrawRectangle(230, y_menu, 154, 90, Fade(DARKGRAY, 0.8f));
                DrawText("Torreta", 230 + 10, y_menu + 10, 16, WHITE);
                DrawText("Custo: 100", 230 + 10, y_menu + 35, 16, WHITE);

                DrawRectangle(420, y_menu, 154, 90, Fade(DARKGRAY, 0.8f));
                DrawText("Muro", 420 + 10, y_menu + 10, 20, WHITE);
                DrawText("Custo: 10", 420 + 10, y_menu + 35, 16, WHITE);

                DrawRectangle(610, y_menu, 154, 90, Fade(DARKGRAY, 0.8f));
                DrawText("Bomba", 610 + 10, y_menu + 10, 20, WHITE);
                DrawText("Custo: 100", 610 + 10, y_menu + 35, 16, WHITE);

                DrawText(TextFormat("Energia: %d", j.energia), 10, 15, 20, WHITE);
                DrawText(TextFormat("Vidas: %d", j.vidas), 200, 15, 20, WHITE);
                DrawText(TextFormat("Score: %d", j.score), 400, 15, 20, WHITE);
                DrawText(TextFormat("Onda atual: %d", j.onda_atual), 600, 15, 20, WHITE);
            EndDrawing();

        // ESTADO 3: GAME OVER
        } else if (estado == 3) {
            BeginDrawing();
                ClearBackground(BLACK);
                DrawTexturePro(fundo, (Rectangle){0, 0, fundo.width, fundo.height}, (Rectangle){0, 0, 800, 600}, (Vector2){0, 0}, 0, WHITE);
                DrawText("GAME OVER", 250, 200, 60, WHITE);
                DrawText(TextFormat("Score: %d", j.score), 300, 290, 30, WHITE);
                DrawText("Clique para jogar novamente", 230, 370, 20, WHITE);

                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    destruir_jogo(&j);
                    iniciar_jogo(&j);
                    for (int i = 0; i < MAX_PROJETEIS; i++) {
                        projeteis[i].ativo = 0;
                    }
                    ultimo_turno = GetTime();
                    defesa_selecionada = -1;
                    estado = 0;
                }
            EndDrawing();
        }
    }

    Score novo_score;
    strcpy(novo_score.nome, nome);
    novo_score.vidas = j.vidas;
    salvar_score(novo_score);
    exibir_scores(scores, scores_salvos);

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