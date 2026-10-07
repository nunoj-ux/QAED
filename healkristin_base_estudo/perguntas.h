#ifndef PERGUNTAS_H
#define PERGUNTAS_H

#include <stddef.h>
#include <stdio.h>

typedef enum {
    TASK_DESCONHECIDA = 0,
    TASK1 = 1,
    TASK2 = 2,
    TASK3 = 3,
    TASK4 = 4
} TipoTask;

typedef struct {
    TipoTask tipo;
    int cidade;
    int valida;         /* Nome, quantidade/formato de argumentos e ID. */
    const char *texto;  /* Emprestado: so valido ate a proxima leitura. */
} Pergunta;

/* APENAS LE E VALIDA. Nao conta clusters nem compara distancias.
   1 = pergunta lida (mesmo mal definida); 0 = EOF; -1 = erro de leitura. */
int perguntas_ler(FILE *ficheiro, int C, Pergunta *pergunta,
                  char **linha, size_t *capacidade);

#endif
