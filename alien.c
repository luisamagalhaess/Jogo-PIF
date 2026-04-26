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
}