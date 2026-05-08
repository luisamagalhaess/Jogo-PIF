#ifndef SCORES_H
#define SCORES_H

typedef struct {
    char nome[50];
    int vidas;
}Score;

void carregar_scores(Score scores[], int* scores_salvos);
void salvar_score(Score novo_score);
void exibir_scores(Score scores[], int scores_salvos);

#endif