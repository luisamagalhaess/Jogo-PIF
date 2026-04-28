#include "alien.h"
#include <stdlib.h>

Alien *criar_alien(int tipo, int linha){
    Alien *a= (Alien *) malloc(sizeof(Alien));
    if (a == NULL){
        return NULL;
    }
    a->tipo = tipo;
    a->linha = linha;
    a->coluna = 8;
    a->next = NULL;

    switch (tipo){
        case ALIEN_INVASOR:
            a->vida = 10;
            a->velocidade = 1;
            a->dano = 10;
            break;
        case ALIEN_BLINDADO:
            a->vida = 100;
            a->velocidade = 1;
            a->dano = 20;
            break;
        case ALIEN_KAMIKAZE:
            a->vida = 10;
            a->velocidade = 2;
            a->dano = 100;
            break;
        }
    return a;
}

void remover_alien(Alien **head, Alien *alien){
    Alien *atual = *head;
    Alien *anterior = NULL;
    while (atual != NULL){
        if(atual == alien){
            if(anterior == NULL){ //se o alien que irá ser removido for o head, o head vira o próximo nó
                *head = atual->next; 
            }else{
                anterior->next = atual->next; // o alien atual vai receber o next
            }
            free(atual);
            return;
        }else{
            anterior = atual;
            atual = atual->next;
        }
    }
}

void destruir_lista(Alien **head){
    while(*head != NULL){
        Alien *temp = (*head)->next;
        free(*head);
        *head = temp;
    }
}