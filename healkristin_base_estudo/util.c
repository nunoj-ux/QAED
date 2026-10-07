#include "util.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Evita que quantidade * tamanho transborde antes da alocacao. */
void *util_alocar_vetor(size_t quantidade, size_t tamanho, int zerar)
{
    if (tamanho == 0 || quantidade > SIZE_MAX / tamanho) {
        return NULL;
    }
    return zerar ? calloc(quantidade, tamanho) : malloc(quantidade * tamanho);
}

/* Le uma linha inteira, mesmo longa ou sem '\n' no fim do ficheiro.
   char ** permite modificar o ponteiro do chamador quando realloc o muda. */
int util_ler_linha(FILE *ficheiro, char **linha, size_t *capacidade)
{
    size_t usados = 0;
    int ch; /* int permite distinguir todos os bytes do valor especial EOF. */

    if (*linha == NULL) {
        *capacidade = 128;
        *linha = malloc(*capacidade);
        if (*linha == NULL) {
            return -1;
        }
    }
    while ((ch = fgetc(ficheiro)) != EOF && ch != '\n') {
        if (ch == '\0') {
            return -1; /* Estes ficheiros sao texto, nao dados binarios. */
        }
        if (usados + 1 >= *capacidade) {
            char *novo;
            size_t nova_capacidade;

            if (*capacidade > SIZE_MAX / 2) {
                return -1;
            }
            nova_capacidade = *capacidade * 2;
            novo = realloc(*linha, nova_capacidade);
            if (novo == NULL) {
                return -1; /* O ponteiro antigo continua disponivel para free. */
            }
            *linha = novo;
            *capacidade = nova_capacidade;
        }
        (*linha)[usados++] = (char)ch;
    }
    if (ferror(ficheiro)) {
        return -1;
    }
    if (ch == EOF && usados == 0) {
        return 0;
    }
    (*linha)[usados] = '\0'; /* Uma string C termina sempre com este byte. */
    return 1;
}

/* Auxiliar privado deste .c: nao aparece em util.h. */
static int linha_vazia(const char *texto)
{
    while (*texto != '\0') {
        if (!isspace((unsigned char)*texto)) {
            return 0;
        }
        ++texto;
    }
    return 1;
}

int util_ler_registo(FILE *ficheiro, char **linha, size_t *capacidade)
{
    int estado;
    do {
        estado = util_ler_linha(ficheiro, linha, capacidade);
    } while (estado == 1 && linha_vazia(*linha));
    return estado;
}

/* Devolve um ponteiro para dentro da linha original; nao aloca memoria. */
char *util_aparar(char *linha)
{
    char *fim;
    while (isspace((unsigned char)*linha)) {
        ++linha;
    }
    fim = linha + strlen(linha);
    while (fim > linha && isspace((unsigned char)fim[-1])) {
        --fim;
    }
    *fim = '\0';
    return linha;
}

/* strtol permite distinguir "0" de "abc" e detetar valores excessivos. */
int util_inteiros(const char *texto, int *valores, size_t n)
{
    size_t i;
    for (i = 0; i < n; ++i) {
        char *fim;
        long valor;
        while (isspace((unsigned char)*texto)) {
            ++texto;
        }
        errno = 0;
        valor = strtol(texto, &fim, 10);
        if (fim == texto || errno == ERANGE || valor < INT_MIN || valor > INT_MAX) {
            return 0;
        }
        if (*fim != '\0' && !isspace((unsigned char)*fim)) {
            return 0; /* Rejeita, por exemplo, 3.5 e 2abc. */
        }
        valores[i] = (int)valor;
        texto = fim;
    }
    while (isspace((unsigned char)*texto)) {
        ++texto;
    }
    return *texto == '\0'; /* Rejeita campos a mais. */
}

int util_tem_extensao(const char *nome, const char *extensao)
{
    size_t a = strlen(nome);
    size_t b = strlen(extensao);
    return a >= b && strcmp(nome + a - b, extensao) == 0;
}

char *util_nome_resultados(const char *nome_quests)
{
    const char *extensao = ".results";
    size_t base;
    size_t bytes;
    char *nome;

    if (!util_tem_extensao(nome_quests, ".quests")) {
        return NULL;
    }
    base = strlen(nome_quests) - strlen(".quests");
    bytes = strlen(extensao) + 1; /* Inclui o terminador '\0'. */
    if (base > SIZE_MAX - bytes) {
        return NULL;
    }
    nome = malloc(base + bytes);
    if (nome != NULL) {
        memcpy(nome, nome_quests, base);
        memcpy(nome + base, extensao, bytes);
    }
    return nome;
}
