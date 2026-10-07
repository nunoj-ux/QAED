#include "grafo.h"
#include "util.h"

#include <limits.h>
#include <stdlib.h>

/* Representacao privada do tipo abstrato Grafo. */
struct Grafo {
    int numero_cidades;
    int numero_ligacoes;      /* Conta as linhas a b, nao os 2L nos. */
    Ligacao **adjacencias;   /* Vetor: cada elemento e uma cabeca de lista. */
    Posicao *posicoes;       /* Vetor: um par (x,y) por cidade. */
};

/* A posicao 0 dos vetores nao e usada: os IDs do enunciado comecam em 1. */
Grafo *grafo_criar(int numero_cidades)
{
    Grafo *g;
    size_t quantidade;
    int i;

    if (numero_cidades <= 0) {
        return NULL;
    }
    g = malloc(sizeof(*g));
    if (g == NULL) {
        return NULL;
    }
    quantidade = (size_t)numero_cidades + 1;
    g->numero_cidades = numero_cidades;
    g->numero_ligacoes = 0;
    g->adjacencias = util_alocar_vetor(quantidade, sizeof(*g->adjacencias), 0);
    g->posicoes = util_alocar_vetor(quantidade, sizeof(*g->posicoes), 1);
    if (g->adjacencias == NULL || g->posicoes == NULL) {
        free(g->adjacencias);
        free(g->posicoes);
        free(g);
        return NULL;
    }
    g->adjacencias[0] = NULL;
    for (i = 0; i < numero_cidades; ++i) {
        g->adjacencias[i + 1] = NULL; /* Inicialmente, todas as listas vazias. */
    }
    return g;
}

/* A rede e nao orientada: uma ligacao a-b cria a->b e b->a. */
int grafo_adicionar_ligacao(Grafo *g, int a, int b)
{
    Ligacao *ida;
    Ligacao *volta;

    if (a < 1 || a > g->numero_cidades || b < 1 || b > g->numero_cidades ||
        g->numero_ligacoes == INT_MAX) {
        return 0;
    }
    ida = malloc(sizeof(*ida));
    volta = malloc(sizeof(*volta));
    if (ida == NULL || volta == NULL) {
        free(ida);
        free(volta);
        return 0; /* As listas ainda nao foram alteradas. */
    }
    /* Inserir na cabeca demora O(1): novo no aponta para a antiga cabeca. */
    ida->destino = b;
    ida->seguinte = g->adjacencias[a];
    g->adjacencias[a] = ida;

    volta->destino = a;
    volta->seguinte = g->adjacencias[b];
    g->adjacencias[b] = volta;
    ++g->numero_ligacoes;
    return 1;
}

int grafo_num_cidades(const Grafo *g)
{
    return g->numero_cidades;
}

int grafo_num_ligacoes(const Grafo *g)
{
    return g->numero_ligacoes;
}

/* Ponteiro emprestado: o chamador percorre a lista, mas nao a liberta. */
const Ligacao *grafo_vizinhos(const Grafo *g, int cidade)
{
    return g->adjacencias[cidade];
}

void grafo_definir_posicao(Grafo *g, int cidade, int x, int y)
{
    g->posicoes[cidade].x = x;
    g->posicoes[cidade].y = y;
}

Posicao grafo_posicao(const Grafo *g, int cidade)
{
    return g->posicoes[cidade]; /* Devolve uma copia da pequena estrutura. */
}

/* Liberta primeiro os nos, depois os vetores e finalmente o proprio grafo. */
void grafo_destruir(Grafo *g)
{
    int i;
    if (g == NULL) {
        return;
    }
    for (i = 0; i < g->numero_cidades; ++i) {
        Ligacao *atual = g->adjacencias[i + 1];
        while (atual != NULL) {
            Ligacao *seguinte = atual->seguinte; /* Guardar antes de free! */
            free(atual);
            atual = seguinte;
        }
    }
    free(g->adjacencias);
    free(g->posicoes);
    free(g);
}
