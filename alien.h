#ifndef ALIEN_H
#define ALIEN_H

#include "tipos.h"

typedef struct Alien{
    int tipo;
    int vida;
    int velocidade;
    int dano;
    int linha;
    int coluna;
    struct Alien *next;
} Alien;

Alien *criar_alien(int tipo, int linha);
void remover_alien(Alien **head, Alien *alien);
void destruir_lista(Alien **head);

#endif