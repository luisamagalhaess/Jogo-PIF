#include "defesa.h"
#include <stdlib.h>

Defesa *criar_defesa(int tipo){
    Defesa *d = (Defesa *) malloc(sizeof(Defesa));
    if (d == NULL){
        return NULL;
    }
    d->tipo = tipo;
    switch (tipo){
        case DEFESA_GERADOR:
            d->vida = 100;
            d->custo = 10;
            d->dano = 0;
            d->cadencia = 0;
            d->energia_por_turno = 25;
            d->raio_explosao = 0;
            break;
        case DEFESA_TORRETA:
            d->vida = 100;
            d->custo = 100;
            d->dano = 10;
            d->cadencia = 2;
            d->energia_por_turno = 0;
            d->raio_explosao = 0;
            break;
        case DEFESA_MURO:
            d->vida = 300;
            d->custo = 10;
            d->dano = 0;
            d->cadencia = 0;
            d->energia_por_turno = 0;
            d->raio_explosao = 0;
            break;
        case DEFESA_BOMBA:
            d->vida = 1;
            d->custo = 100;
            d->dano = 200;
            d->cadencia = 0;
            d->energia_por_turno = 0;
            d->raio_explosao = 1;
            break;
    }
    return d;
}

void destruir_defesa(Defesa *d){
    if (d != NULL) {
        free(d);
    }
}