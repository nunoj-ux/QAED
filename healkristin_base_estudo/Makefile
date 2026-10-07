# Compilar: make. Ver os dados: make demo. Limpar: make clean.
# As linhas com comandos comecam com TAB, nao com espacos.
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic -O0 -g
CPPFLAGS =
LDFLAGS =
LDLIBS =

SOURCES = main.c grafo.c leitura.c perguntas.c util.c
OBJECTS = $(SOURCES:.c=.o)
DEPS = $(OBJECTS:.o=.d)
HEADERS = grafo.h leitura.h perguntas.h util.h

QUESTS ?= exemplos/enunciado.quests
MAP ?= exemplos/enunciado.map
POSITION ?= exemplos/enunciado.position

.PHONY: all run demo clean
all: healkristin

# Linker: junta os objetos para criar o programa.
healkristin: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

# Compilador: um .c gera um .o; -MMD -MP regista dependencias dos .h.
%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

# Executavel separado: -DDEMONSTRAR inclui os blocos #ifdef do main.
healkristin_demo: $(SOURCES) $(HEADERS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DDEMONSTRAR $(LDFLAGS) -o $@ $(SOURCES) $(LDLIBS)

run: healkristin
	./healkristin "$(QUESTS)" "$(MAP)" "$(POSITION)"

demo: healkristin_demo
	./healkristin_demo "$(QUESTS)" "$(MAP)" "$(POSITION)"

clean:
	rm -f healkristin healkristin_demo $(OBJECTS) $(DEPS)

-include $(DEPS)
