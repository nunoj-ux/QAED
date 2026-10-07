#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>
#include <stdio.h>

/* Auxiliares comuns: nao resolvem qualquer Task. */
void *util_alocar_vetor(size_t quantidade, size_t tamanho, int zerar);
/* 1 = linha lida; 0 = fim do ficheiro; -1 = erro. */
int util_ler_linha(FILE *ficheiro, char **linha, size_t *capacidade);
/* Igual a ler_linha, mas salta linhas compostas apenas por espacos. */
int util_ler_registo(FILE *ficheiro, char **linha, size_t *capacidade);
char *util_aparar(char *linha);
/* Exige exatamente n inteiros. n == 0 exige texto vazio/espacos. */
int util_inteiros(const char *texto, int *valores, size_t n);
int util_tem_extensao(const char *nome, const char *extensao);
/* Devolve memoria nova: quem chama deve fazer free. Nao cria ficheiros. */
char *util_nome_resultados(const char *nome_quests);

#endif
