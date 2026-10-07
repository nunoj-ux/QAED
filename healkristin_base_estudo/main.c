#include <stdio.h>
#include <stdlib.h>

#include "grafo.h"
#include "leitura.h"
#include "perguntas.h"
#include "util.h"

#ifdef DEMONSTRAR
/* So existe no executavel de estudo, construido por make demo.
   Mostra dados lidos, nunca respostas calculadas para as Tasks. */
static void mostrar_dados(const Grafo *g, const char *nome_resultados)
{
    int C = grafo_num_cidades(g);
    int i;
    printf("BASE DE ESTUDO - Tasks ainda nao implementadas\n");
    printf("Cidades: %d | Ligacoes: %d\n", C, grafo_num_ligacoes(g));
    printf("Futuro .results (nao criado): %s\n", nome_resultados);
    for (i = 0; i < C; ++i) {
        int cidade = i + 1;
        Posicao p = grafo_posicao(g, cidade);
        const Ligacao *no = grafo_vizinhos(g, cidade);
        printf("Cidade %d | (%d,%d) | vizinhos:", cidade, p.x, p.y);
        if (no == NULL) {
            printf(" nenhum");
        }
        while (no != NULL) {
            printf(" %d", no->destino);
            no = no->seguinte;
        }
        printf("\n");
    }
}
#endif

/* Main coordena: argumentos -> ficheiros -> rede -> perguntas -> limpeza. */
int main(int argc, char *argv[])
{
    FILE *quests = NULL;
    FILE *mapa = NULL;
    FILE *posicoes = NULL;
    Grafo *g = NULL;
    char *nome_resultados = NULL;
    char *linha = NULL;
    size_t capacidade = 0;
    Pergunta pergunta;
    int estado_leitura;
    int resultado = EXIT_FAILURE;

    /* argv[0] e o programa; os tres ficheiros sao argv[1], [2] e [3].
       || usa curto-circuito: se argc != 4, nao lemos argv inexistentes. */
    if (argc != 4 || !util_tem_extensao(argv[1], ".quests") ||
        !util_tem_extensao(argv[2], ".map") ||
        !util_tem_extensao(argv[3], ".position")) {
        return EXIT_FAILURE; /* Sem mensagens, como pede o enunciado. */
    }
    quests = fopen(argv[1], "r");
    mapa = fopen(argv[2], "r");
    posicoes = fopen(argv[3], "r");
    if (quests == NULL || mapa == NULL || posicoes == NULL) {
        goto fim;
    }
    g = leitura_mapa(mapa);
    if (g == NULL || !leitura_posicoes(posicoes, g)) {
        goto fim;
    }
    nome_resultados = util_nome_resultados(argv[1]);
    if (nome_resultados == NULL) {
        goto fim;
    }

    /* PARAR AQUI A LOGICA DE RESOLUCAO, conforme o pedido.
       Futuramente: calcular clusters uma vez e abrir nome_resultados.
       Esta base NAO abre nem cria qualquer ficheiro .results. */
#ifdef DEMONSTRAR
    mostrar_dados(g, nome_resultados);
#endif
    while ((estado_leitura = perguntas_ler(quests, grafo_num_cidades(g),
               &pergunta, &linha, &capacidade)) == 1) {
        /* Futuramente: escolher a Task e escrever a resposta.
           Agora apenas reconhecemos e validamos a pergunta. */
#ifdef DEMONSTRAR
        printf("Pedido lido: %s | %s\n", pergunta.texto,
               pergunta.valida ? "formato e ID validos" : "mal definido");
#endif
    }
    if (estado_leitura == 0) {
        resultado = EXIT_SUCCESS;
    }

fim:
    /* Um unico caminho de limpeza, tanto em sucesso como em erro. */
    if (quests != NULL) {
        fclose(quests);
    }
    if (mapa != NULL) {
        fclose(mapa);
    }
    if (posicoes != NULL) {
        fclose(posicoes);
    }
    free(linha); /* Nao fazer free(pergunta.texto): aponta para dentro de linha. */
    free(nome_resultados);
    grafo_destruir(g);
    return resultado;
}
