#ifndef JOGO_H
#define JOGO_H

#include "tipos.h"
#include "defesa.h"
#include "alien.h"

typedef struct {
    Defesa *defesa;
} Celula;

typedef struct {
    Celula grid[LINHAS][COLUNAS];
    Alien* aliens[LINHAS];
    int energia;
    int onda_atual;
    int score;
    int vidas;
    int cursor_linha;
    int cursor_coluna;
} Jogo;

void iniciar_jogo(Jogo* j);
void destruir_jogo(Jogo* j);
void mover_aliens(Jogo *j);
void torretas_atacam(Jogo *j);
void geradores_produzem(Jogo *j);
void spawnar_alien(Jogo *j);

#endif