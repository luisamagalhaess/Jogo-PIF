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

void exibir_scores(Score scores[], int scores_salvos){

    for (int i = 0; i < scores_salvos - 1; i++) {
        for (int j = 0; j < scores_salvos - i - 1; j++) {
            if (scores[j].vidas < scores[j+1].vidas) {
                Score temp = scores[j];      
                scores[j] = scores[j+1];    
                scores[j+1] = temp;          
            }
        }
    }
    
    for (int i = 0; i < scores_salvos; i++) {
        printf("%dº - %s - %d vidas\n", i+1, scores[i].nome, scores[i].vidas);
    }
}
