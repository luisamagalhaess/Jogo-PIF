#ifndef DEFESA_H
#define DEFESA_H

#include "tipos.h"

typedef struct Defesa{
    int tipo;
    int vida;
    int custo;
    int dano;
    int cadencia;
    int energia_por_turno;
    int raio_explosao;
}Defesa;

Defesa *criar_defesa(int tipo);
void destruir_defesa(Defesa *d);

#endif