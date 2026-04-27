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