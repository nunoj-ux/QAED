#ifndef LEITURA_H
#define LEITURA_H

#include <stdio.h>
#include "grafo.h"

/* Constroi o grafo a partir do .map; NULL significa erro. */
Grafo *leitura_mapa(FILE *ficheiro);
/* Guarda coordenadas validadas; 1 = sucesso, 0 = erro. */
int leitura_posicoes(FILE *ficheiro, Grafo *grafo);

#endif
