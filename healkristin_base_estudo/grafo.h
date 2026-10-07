#ifndef GRAFO_H
#define GRAFO_H

/* Um no da lista guarda um vizinho, nao a cidade de origem. */
typedef struct Ligacao {
    int destino;
    struct Ligacao *seguinte;
} Ligacao;

typedef struct {
    int x;
    int y;
} Posicao;

/* Tipo opaco: os campos do Grafo so sao conhecidos em grafo.c. */
typedef struct Grafo Grafo;

Grafo *grafo_criar(int numero_cidades);
void grafo_destruir(Grafo *grafo);
/* Devolve 1 se inseriu os dois sentidos; 0 se IDs invalidos/memoria falhou. */
int grafo_adicionar_ligacao(Grafo *grafo, int a, int b);
int grafo_num_cidades(const Grafo *grafo);
int grafo_num_ligacoes(const Grafo *grafo);
/* As tres operacoes seguintes pressupoem cidade entre 1 e C. */
const Ligacao *grafo_vizinhos(const Grafo *grafo, int cidade);
void grafo_definir_posicao(Grafo *grafo, int cidade, int x, int y);
Posicao grafo_posicao(const Grafo *grafo, int cidade);

#endif
