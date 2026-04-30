#include "scores.h"
#include <stdio.h>

void salvar_score(Score novo_score) {
    FILE *arquivo = fopen("scores.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de scores.\n");
        return;
    }
    fprintf(arquivo, "%s %d\n", novo_score.nome, novo_score.vidas);
    fclose(arquivo);
}

void carregar_scores(Score scores[], int* scores_salvos) {
    FILE *arquivo = fopen(  "scores.txt", "r");
    if (arquivo == NULL) {
        printf("Nenhum score salvo encontrado.\n");
        *scores_salvos = 0;
        return;
    }
    *scores_salvos = 0;
    while (fscanf(arquivo, "%s %d", scores[*scores_salvos].nome, &scores[*scores_salvos].vidas) ==2) {
        (*scores_salvos)++;
    }
    fclose(arquivo);
    
}