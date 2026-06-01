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
    SetTargetFPS(60);
    iniciar_jogo(&j);
    srand(time(NULL));
    carregar_scores(scores, &scores_salvos);

    int x_inicial = (800 - 9 * 64) / 2;
    int y_inicial = 140;
    double ultimo_turno = GetTime();
    double intervalo = 4.5;
    Texture2D fundo = LoadTexture("assets/fundo.png");
    Texture2D alien_blindado = LoadTexture("assets/alien_blindado.png");
    Texture2D alien_kamikaze = LoadTexture("assets/alien_kamikaze.png");
    Texture2D gerador = LoadTexture("assets/gerador.png");
    Texture2D muro = LoadTexture("assets/muro.png");
    Texture2D torreta = LoadTexture("assets/torreta.png");
    Texture2D bomba = LoadTexture("assets/bomba.png");
    Texture2D alien_invasor = LoadTexture("assets/alien_invasor.png");
    Texture2D cartas = LoadTexture("assets/cartas.png");
    Texture2D slide1 = LoadTexture("assets/slide1.png");
    Texture2D slide2 = LoadTexture("assets/slide2.png");
    Texture2D slide3 = LoadTexture("assets/slide3.png");

    const char *textos_slides[3] = {
        "Os aliens descobriram o planeta Erid e cobiçaram seus recursos...",
        "Sem aviso, iniciaram um bombardeio devastador contra o planeta...",
        "As defesas de Erid foram destruidas. Apenas voce ainda resiste..."
    };
    int slide_atual = 0;
    int char_atual = 0;
    double ultimo_char = GetTime();
    double tempo_inicio_slide = GetTime();
    
    int defesa_selecionada = -1;
    int y_menu = 465;
    int altura_cartas = 600 - y_menu;
    int estado = 0;
    Projetil projeteis[MAX_PROJETEIS] = {0};
    
    while (!WindowShouldClose()) {

        if (estado == 0) {
            Texture2D slide_tex;
            if (slide_atual == 0) slide_tex = slide1;
            else if (slide_atual == 1) slide_tex = slide2;
            else slide_tex = slide3;

            float tempo_slide = (float)(GetTime() - tempo_inicio_slide);
            float zoom = 1.0f - (tempo_slide * 0.02f);
            if (zoom < 0.85f) zoom = 0.85f;
            float largura_crop = slide_tex.width * zoom;
            float altura_crop = slide_tex.height * zoom;
            float x_crop = (slide_tex.width - largura_crop) / 2;
            float y_crop = (slide_tex.height - altura_crop) / 2;

            BeginDrawing();
                ClearBackground(BLACK);
                DrawTexturePro(slide_tex,
                    (Rectangle){x_crop, y_crop, largura_crop, altura_crop},
                    (Rectangle){0, 0, 800, 600},
                    (Vector2){0, 0}, 0, WHITE);
                DrawRectangle(0, 450, 800, 150, Fade(BLACK, 0.7f));
                DrawText("Clique para continuar...", 300, 540, 16, GRAY);

                if (GetTime() - ultimo_char >= 0.05) {
                    if (char_atual < (int)strlen(textos_slides[slide_atual])) {
                        char_atual++;
                    }
                    ultimo_char = GetTime();
                }

                int largura_texto = MeasureText(textos_slides[slide_atual], 18);
                int x_texto = (800 - largura_texto) / 2;
                DrawText(TextSubtext(textos_slides[slide_atual], 0, char_atual), x_texto, 470, 18, WHITE);

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    slide_atual++;
                    char_atual = 0;
                    ultimo_char = GetTime();
                    tempo_inicio_slide = GetTime();
                    if (slide_atual >= 3) estado = 1;
                }
            EndDrawing();

        } else if (estado == 1) {
            Vector2 m = GetMousePosition();
            int y_botao = 360;
            Color cor_borda = WHITE;

            if (m.x >= 280 && m.x <= 520 && m.y >= 360 && m.y <= 420) {
                y_botao = 352;
                cor_borda = YELLOW;
            }

            Color cor_borda_cj = WHITE;
            int y_botao_cj = 450;
            if (m.x >= 280 && m.x <= 520 && m.y >= 450 && m.y <= 510) {
                y_botao_cj = 442;
                cor_borda_cj = YELLOW;
            }

            BeginDrawing();
                ClearBackground(BLACK);
                DrawTexturePro(fundo, (Rectangle){0, 0, fundo.width, fundo.height}, (Rectangle){0, 0, 800, 600}, (Vector2){0, 0}, 0, WHITE);
                DrawText("ERID", 280, 180, 80, WHITE);
                DrawText("Defesa do Planeta", 230, 270, 25, LIGHTGRAY);

                DrawRectangleRounded((Rectangle){280, y_botao, 240, 60}, 0.3f, 8, Fade(DARKGRAY, 0.8f));
                DrawRectangleRoundedLines((Rectangle){280, y_botao, 240, 60}, 0.3f, 8, cor_borda);
                DrawText("Iniciar Batalha", 305, y_botao + 18, 22, WHITE);

                DrawRectangleRounded((Rectangle){280, y_botao_cj, 240, 60}, 0.3f, 8, Fade(DARKGRAY, 0.8f));
                DrawRectangleRoundedLines((Rectangle){280, y_botao_cj, 240, 60}, 0.3f, 8, cor_borda_cj);
                DrawText("Como Jogar", 315, y_botao_cj + 18, 22, WHITE);

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (m.x >= 280 && m.x <= 520 && m.y >= y_botao && m.y <= y_botao + 60) {
                        estado = 2;
                        ultimo_turno = GetTime();
                    }
                    if (m.x >= 280 && m.x <= 520 && m.y >= y_botao_cj && m.y <= y_botao_cj + 60) {
                        estado = 5;
                    }
                }
            EndDrawing();

        } else if (estado == 2) {
            if (j.vidas <= 0) estado = 3;
            else if (j.onda_atual > TOTAL_ONDAS && !aliens_presentes(&j)) estado = 4;

            Vector2 mouse = GetMousePosition();
            int col_mouse = (mouse.x - x_inicial) / 64;
            int lin_mouse = (mouse.y - y_inicial) / 64;

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                if(mouse.y >= y_menu){
                    if(mouse.x >= 150 && mouse.x <= 275){
                        defesa_selecionada = DEFESA_GERADOR;
                    }else if(mouse.x >= 275 && mouse.x <= 400){
                        defesa_selecionada = DEFESA_TORRETA;
                    }else if(mouse.x >= 400 && mouse.x <= 525){
                        defesa_selecionada = DEFESA_MURO;
                    }else if(mouse.x >= 525 && mouse.x <= 650){
                        defesa_selecionada = DEFESA_BOMBA;
                    }
                }else{
                    j.cursor_linha = lin_mouse;
                    j.cursor_coluna = col_mouse;
                    if (defesa_selecionada != -1){
                        int custo = 0;
                        if(defesa_selecionada == DEFESA_GERADOR) custo = 100;
                        else if(defesa_selecionada == DEFESA_TORRETA) custo = 200;
                        else if(defesa_selecionada == DEFESA_MURO) custo = 100;
                        else if(defesa_selecionada == DEFESA_BOMBA) custo = 300;

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

            for (int i = 0; i < MAX_PROJETEIS; i++) {
                if (!projeteis[i].ativo) continue;

                float t = (float)((GetTime() - projeteis[i].tempo_inicio) / projeteis[i].duracao);

                if (t >= 1.0f) {
                    projeteis[i].ativo = 0;
                    continue;
                }

                float px = projeteis[i].x_origem + (projeteis[i].x_destino - projeteis[i].x_origem) * t;
                float py = projeteis[i].y_origem + (projeteis[i].y_destino - projeteis[i].y_origem) * t;

                int linha = projeteis[i].linha;
                Alien *a = j.aliens[linha];
                while (a != NULL) {
                    Alien *proximo = a->next;

                    float coluna_visual = (a->coluna + 1) - progresso_turno;
                    float ax = (float)(x_inicial + coluna_visual * 64 + 32);
                    float ay = (float)(y_inicial + linha * 64 + 32);

                    float dx = px - ax;
                    float dy = py - ay;

                    if (dx*dx + dy*dy < 20.0f * 20.0f) {
                        a->vida -= projeteis[i].dano;
                        a->tempo_hit = GetTime();
                        projeteis[i].ativo = 0;

                        if (a->vida <= 0) {
                            j.score += 10;
                            j.aliens_mortos++;
                            remover_alien(&j.aliens[linha], a);
                        }
                        break;
                    }
                    a = proximo;
                }
            }

            BeginDrawing();
                ClearBackground(BLACK);
                DrawTexturePro(fundo, (Rectangle){0, 0, fundo.width, fundo.height}, (Rectangle){0, 0, 800, 600}, (Vector2){0, 0}, 0, WHITE);

                for (int l = 0; l < LINHAS; l++) {
                    for (int c = 0; c < COLUNAS; c++) {
                        Color cor = BLANK;

                        if (l == lin_mouse && c == col_mouse) {
                            cor = Fade(WHITE, 0.2f);
                        }
                        if (l == j.cursor_linha && c == j.cursor_coluna){
                            cor = Fade(YELLOW, 0.4f);
                        }

                        if (cor.a > 0) {
                            DrawRectangle(x_inicial + c * 64, y_inicial + l * 64, 64, 64, cor);
                        }

                        DrawRectangle(x_inicial + c * 64, y_inicial + l * 64, 64, 64, Fade(BLACK, 0.4f));
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

                        Color tint = RAYWHITE;
                        float tempo_desde_hit = (float)(GetTime() - a->tempo_hit);
                        float flash_duracao = 0.6f;

                        if (a->tempo_hit > 0 && tempo_desde_hit < flash_duracao) {
                            float intensidade = fabsf(sinf(tempo_desde_hit * 10.0f));
                            intensidade *= (1.0f - tempo_desde_hit / flash_duracao);
                            tint = (Color){
                                255,
                                (unsigned char)(255.0f * (1.0f - intensidade * 0.85f)),
                                (unsigned char)(255.0f * (1.0f - intensidade * 0.85f)),
                                255
                            };
                        }

                        DrawTexturePro(tex,
                            (Rectangle){frame_atual * frame_largura, 0, frame_largura, tex.height},
                            (Rectangle){x_inicial + coluna_visual * 64, y_inicial + l * 64, 64, 64},
                            (Vector2){0, 0}, 0, tint);
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
                            int num_frames_defesa = 4;
                            int frame_largura_defesa = tex.width / num_frames_defesa;
                            int frame_atual_defesa = (int)(GetTime() * 6) % num_frames_defesa;

                            DrawTexturePro(
                                tex,
                                (Rectangle){frame_atual_defesa * frame_largura_defesa, 0, frame_largura_defesa, tex.height},
                                (Rectangle){x_inicial + c * 64, y_inicial + l * 64, 64, 64},
                                (Vector2){0, 0}, 0, RAYWHITE
                            );
                        }
                    }
                }

                DrawTexturePro(cartas, (Rectangle){0, 0, cartas.width, cartas.height},
                                (Rectangle){150, y_menu, 500, altura_cartas},
                                (Vector2){0, 0}, 0, WHITE);

                if(defesa_selecionada == DEFESA_GERADOR){
                    DrawRectangle(150, y_menu, 125, altura_cartas, Fade(YELLOW, 0.3f));
                }else if(defesa_selecionada == DEFESA_TORRETA){
                    DrawRectangle(275, y_menu, 125, altura_cartas, Fade(YELLOW, 0.3f));
                }else if(defesa_selecionada == DEFESA_MURO){
                    DrawRectangle(400, y_menu, 125, altura_cartas, Fade(YELLOW, 0.3f));
                }else if(defesa_selecionada == DEFESA_BOMBA){
                    DrawRectangle(525, y_menu, 125, altura_cartas, Fade(YELLOW, 0.3f));
                }

                for (int i = 0; i < MAX_PROJETEIS; i++) {
                    if (!projeteis[i].ativo) continue;

                    float t = (float)((GetTime() - projeteis[i].tempo_inicio) / projeteis[i].duracao);
                    if (t >= 1.0f) continue;

                    float px = projeteis[i].x_origem + (projeteis[i].x_destino - projeteis[i].x_origem) * t;
                    float py = projeteis[i].y_origem + (projeteis[i].y_destino - projeteis[i].y_origem) * t;

                    DrawCircle((int)px, (int)py, 9, Fade(YELLOW, 0.35f));
                    DrawCircle((int)px, (int)py, 4, YELLOW);
                }

                DrawText(TextFormat("Energia: %d", j.energia), 10, 15, 20, WHITE);
                DrawText(TextFormat("Vidas: %d", j.vidas), 200, 15, 20, WHITE);
                DrawText(TextFormat("Score: %d", j.score), 400, 15, 20, WHITE);
                DrawText(TextFormat("Onda atual: %d", j.onda_atual), 600, 15, 20, WHITE);
            EndDrawing();

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
                    slide_atual = 0;
                    tempo_inicio_slide = GetTime();
                    estado = 0;
                }
            EndDrawing();

        } else if (estado == 4){
            BeginDrawing();
                ClearBackground(BLACK);
                DrawTexturePro(fundo, (Rectangle){0, 0, fundo.width, fundo.height}, (Rectangle){0, 0, 800, 600}, (Vector2){0, 0}, 0, WHITE);
                DrawText("VITÓRIA!", 250, 200, 60, WHITE);
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
                    slide_atual = 0;
                    tempo_inicio_slide = GetTime();
                    estado = 0;
                }
            EndDrawing();

        } else if (estado == 5) {
            BeginDrawing();
                ClearBackground(BLACK);
                DrawTexturePro(fundo, (Rectangle){0, 0, fundo.width, fundo.height}, (Rectangle){0, 0, 800, 600}, (Vector2){0, 0}, 0, Fade(WHITE, 0.4f));

                DrawText("COMO JOGAR", 270, 20, 30, WHITE);

                DrawText("DEFESAS:", 40, 70, 20, YELLOW);
                DrawText("Gerador  - Custo: 100 | Gera 25 de energia por turno", 40, 100, 16, WHITE);
                DrawText("Torreta  - Custo: 200 | Ataca o alien mais proximo a cada 2 turnos", 40, 125, 16, WHITE);
                DrawText("Muro     - Custo: 100 | Bloqueia e absorve dano (vida: 300)", 40, 150, 16, WHITE);
                DrawText("Bomba    - Custo: 300 | Explode em area ao ser atingida (dano: 200)", 40, 175, 16, WHITE);

                DrawText("ALIENS:", 40, 220, 20, YELLOW);
                DrawText("Invasor  - Vida: 100 | Dano: 10  | Alien padrao", 40, 250, 16, WHITE);
                DrawText("Blindado - Vida: 500 | Dano: 20  | Alta resistencia", 40, 275, 16, WHITE);
                DrawText("Kamikaze - Vida: 50  | Dano: 100 | Rapido e altamente destrutivo", 40, 300, 16, WHITE);

                DrawText("CONTROLES:", 40, 345, 20, YELLOW);
                DrawText("Clique nas cartas para selecionar uma defesa", 40, 375, 16, WHITE);
                DrawText("Clique numa celula do grid para posicionar a defesa", 40, 400, 16, WHITE);
                DrawText("Se um alien chegar ao lado esquerdo do grid, voce perde uma vida", 40, 425, 16, WHITE);

                Vector2 mv = GetMousePosition();
                Color cor_voltar = WHITE;
                int y_voltar = 500;
                if (mv.x >= 280 && mv.x <= 520 && mv.y >= 500 && mv.y <= 560) {
                    y_voltar = 492;
                    cor_voltar = YELLOW;
                }
                DrawRectangleRounded((Rectangle){280, y_voltar, 240, 60}, 0.3f, 8, Fade(DARKGRAY, 0.8f));
                DrawRectangleRoundedLines((Rectangle){280, y_voltar, 240, 60}, 0.3f, 8, cor_voltar);
                DrawText("Voltar", 370, y_voltar + 18, 22, WHITE);

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (mv.x >= 280 && mv.x <= 520 && mv.y >= y_voltar && mv.y <= y_voltar + 60) {
                        estado = 1;
                    }
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
    UnloadTexture(cartas);
    UnloadTexture(alien_invasor);
    UnloadTexture(alien_blindado);
    UnloadTexture(alien_kamikaze);
    UnloadTexture(gerador);
    UnloadTexture(muro);
    UnloadTexture(torreta);
    UnloadTexture(bomba);
    UnloadTexture(slide1);
    UnloadTexture(slide2);
    UnloadTexture(slide3);
    CloseWindow();
    destruir_jogo(&j);
    return 0;
}