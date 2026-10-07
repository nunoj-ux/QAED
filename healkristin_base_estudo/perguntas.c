#include "perguntas.h"
#include "util.h"

#include <ctype.h>
#include <string.h>

/* Reconhece Task1-Task4 sem confundir Task1 com Task10 ou Task1abc. */
static TipoTask identificar(const char *texto)
{
    static const char *const nomes[] = {"Task1", "Task2", "Task3", "Task4"};
    int i;
    for (i = 0; i < 4; ++i) {
        if (strncmp(texto, nomes[i], 5) == 0 &&
            (texto[5] == '\0' || isspace((unsigned char)texto[5]))) {
            return (TipoTask)(i + 1);
        }
    }
    return TASK_DESCONHECIDA;
}

/* Mantem perguntas invalidas: futuramente terao resposta -2.
   Uma coordenada invalida, pelo contrario, aborta a leitura da rede. */
int perguntas_ler(FILE *ficheiro, int C, Pergunta *p,
                  char **linha, size_t *capacidade)
{
    int estado = util_ler_registo(ficheiro, linha, capacidade);
    if (estado != 1) {
        return estado;
    }
    p->texto = util_aparar(*linha);
    p->tipo = identificar(p->texto);
    p->cidade = 0;
    p->valida = 0;

    if (p->tipo == TASK1 || p->tipo == TASK2) {
        p->valida = util_inteiros(p->texto + 5, NULL, 0);
    } else if (p->tipo == TASK3 || p->tipo == TASK4) {
        p->valida = util_inteiros(p->texto + 5, &p->cidade, 1) &&
                    p->cidade >= 1 && p->cidade <= C;
    }
    /* Ainda nao conseguimos detetar pedidos impossiveis por haver um
       unico cluster: isso exige o algoritmo da Task1, que nao esta aqui. */
    return 1;
}
