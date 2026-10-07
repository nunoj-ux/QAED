#include "leitura.h"
#include "util.h"

#include <stdlib.h>

/* Le C e L e depois exatamente L pares de cidades. */
Grafo *leitura_mapa(FILE *ficheiro)
{
    char *linha = NULL;
    size_t capacidade = 0;
    Grafo *g = NULL;
    int cabecalho[2];
    int cidades[2];
    int i;

    if (util_ler_registo(ficheiro, &linha, &capacidade) != 1 ||
        !util_inteiros(linha, cabecalho, 2) ||
        cabecalho[0] <= 0 || cabecalho[1] < 0) {
        goto erro;
    }
    g = grafo_criar(cabecalho[0]);
    if (g == NULL) {
        goto erro;
    }
    for (i = 0; i < cabecalho[1]; ++i) {
        if (util_ler_registo(ficheiro, &linha, &capacidade) != 1 ||
            !util_inteiros(linha, cidades, 2) ||
            !grafo_adicionar_ligacao(g, cidades[0], cidades[1])) {
            goto erro;
        }
    }
    /* Depois das L ligacoes, so se admitem linhas vazias e EOF. */
    if (util_ler_registo(ficheiro, &linha, &capacidade) != 0) {
        goto erro;
    }
    free(linha);
    return g; /* O main passa a ser responsavel por destruir este grafo. */

erro:
    free(linha);
    grafo_destruir(g); /* Tambem funciona se g ainda for NULL. */
    return NULL;
}

/* O .position pode listar os IDs em qualquer ordem, mas todos uma vez. */
int leitura_posicoes(FILE *ficheiro, Grafo *g)
{
    char *linha = NULL;
    size_t capacidade = 0;
    unsigned char *vistas = NULL;
    int limites[2];
    int dados[3];
    int C = grafo_num_cidades(g);
    int lidas = 0;
    int estado;
    int sucesso = 0;

    if (util_ler_registo(ficheiro, &linha, &capacidade) != 1 ||
        !util_inteiros(linha, limites, 2) || limites[0] <= 0 || limites[1] <= 0) {
        goto fim;
    }
    /* calloc inicializa os bytes a zero: nenhuma cidade vista. */
    vistas = util_alocar_vetor((size_t)C + 1, sizeof(*vistas), 1);
    if (vistas == NULL) {
        goto fim;
    }
    while ((estado = util_ler_registo(ficheiro, &linha, &capacidade)) == 1) {
        int cidade;
        int x;
        int y;
        if (!util_inteiros(linha, dados, 3)) {
            goto fim;
        }
        cidade = dados[0];
        x = dados[1];
        y = dados[2];
        /* Validar ID ANTES de usar vistas[cidade] ou posicoes[cidade]. */
        if (cidade < 1 || cidade > C || x < 1 || x > limites[0] ||
            y < 1 || y > limites[1] || vistas[cidade]) {
            goto fim;
        }
        vistas[cidade] = 1;
        ++lidas;
        grafo_definir_posicao(g, cidade, x, y);
    }
    /* EOF normal e C IDs distintos validos garantem que nao falta nenhum. */
    sucesso = estado == 0 && lidas == C;

fim:
    free(vistas);
    free(linha);
    return sucesso;
}
