# HealkrISTin: base em C, antes das tasks

Esta é uma base de estudo nova, feita a partir do enunciado fornecido.
Implementa a infraestrutura de entrada. Os algoritmos das Task1, Task2,
Task3 e Task4 ficam por fazer, conforme o pedido.

## Começar

Extrai o ZIP e abre um terminal na pasta que tem o Makefile.

```bash
make
make demo
```

`make` compila `healkristin`. `make demo` compila e executa
`healkristin_demo`, que mostra os dados lidos. Não exige Python, C++,
bibliotecas externas ou um ambiente de execução diferente de C.
É necessário ter GCC e GNU Make disponíveis.

Para o modo normal, silencioso:

```bash
./healkristin exemplos/enunciado.quests exemplos/enunciado.map exemplos/enunciado.position
```

Para usar outros ficheiros no modo de demonstração:

```bash
make demo QUESTS=pedido.quests MAP=rede.map POSITION=locais.position
```

O modo de demonstração mostra o grafo carregado, as coordenadas e a
validação dos pedidos. A ordem dos vizinhos pode diferir da ordem do
ficheiro, porque cada ligação é inserida na cabeça da lista.

O modo normal não imprime texto. Nenhum dos modos cria, abre ou altera
um `.results`: as respostas ainda não são calculadas. O nome futuro de
saída é preparado apenas como string. Um `.results` existente é preservado.
Por isso, esta base ainda não é uma solução funcional da primeira fase.

## O que está feito

- Validação de três argumentos e extensões na ordem obrigatória.
- Abertura dos ficheiros e saída silenciosa em erros.
- Leitura de C e L e de exatamente L ligações válidas.
- Grafo não orientado com listas de adjacência e coordenadas por cidade.
- Coordenadas naturais, dentro dos limites, para todos os IDs 1 a C.
- Leitura de pedidos Task1–Task4, formato e IDs; pedidos mal definidos
  são conservados para receberem futuramente `-2`.
- Limpeza da memória e fecho dos ficheiros.
- Makefile, comentários e demonstração dos dados carregados.

## O que falta

Identificar clusters com DFS/BFS, organizar os seus membros, calcular
proximidades, produzir respostas e escrever o `.results`.
Um pedido pode ter formato e ID válidos e ser impossível por existir só
um cluster; essa verificação pertence aos algoritmos futuros.

## Módulos

| Ficheiro | Responsabilidade |
|---|---|
| `main.c` | Coordenação e limpeza; demonstração opcional |
| `grafo.c` / `grafo.h` | Tipo abstrato Grafo e suas operações |
| `leitura.c` / `leitura.h` | `.map` e `.position` |
| `perguntas.c` / `perguntas.h` | Leitura e validação dos pedidos |
| `util.c` / `util.h` | Texto, inteiros, extensões e memória |
| `Makefile` | Compilação e execução |
| `GUIA_ORAL.md` / `GUIA_ORAL.pdf` | Estudo e explicação das tasks |

Todos os `.c`, `.h` e o Makefile estão na raiz. `make clean` remove
apenas produtos de compilação. Os ZIPs finais não incluem executáveis,
objetos ou scripts em outras linguagens.

## Escolhas desta base

Ignoram-se linhas vazias. Há um único registo de coordenadas por cidade,
em qualquer ordem. Repetições são rejeitadas. Cada linha de dados tem o
número exato de inteiros, todos representáveis em `int` (32 bits no
ambiente verificado). Os limites e as coordenadas começam em 1.
Ligações repetidas e auto-ligações são aceites e guardadas como aparecem;
uma futura DFS com marcação continua a funcionar.
Pedidos desconhecidos, argumentos a mais ou em falta e IDs inválidos
são marcados como mal definidos, sem abortar a leitura dos outros pedidos.

O enunciado fornecido não define um desempate para proximidades. O guia
explica a opção de escolher o menor ID externo; a base não calcula
distâncias nem toma decisões de desempate.

## Verificação

A base passou 78 casos no modo normal e os mesmos 78 no modo de
demonstração. Incluem 30 grafos aleatórios com comparação dos dados
carregados, erros de invocação, mapas e coordenadas inválidos, perguntas
mal definidas, CRLF, linhas longas, ausência de newline final e uma cadeia
de 20 000 cidades. A mesma bateria passou no modo de demonstração com
AddressSanitizer e UndefinedBehaviorSanitizer. A deteção de fugas estava
desligada nesses testes; não se afirma uma verificação dinâmica de fugas.
Não houve verificação no Mooshak nem testes de algoritmos das tasks,
porque estes não estão implementados.
