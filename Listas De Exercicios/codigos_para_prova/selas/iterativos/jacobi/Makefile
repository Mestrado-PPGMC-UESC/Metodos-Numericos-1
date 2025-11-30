# ==== Exercicio1/Metodos Iterativos/jacobi ====

ifeq ($(OS),Windows_NT)
  EXE  := jacobi.exe
  RUN  := $(EXE)          # em cmd.exe, basta chamar o nome do exe
  SHELL := cmd
  RM   := del /Q
  NULL := nul
else
  EXE  := jacobi
  RUN  := ./$(EXE)        # em Unix, prefixo ./ é o correto
  RM   := rm -f
  NULL := /dev/null
endif

CC      := gcc
CFLAGS  := -Wall -Wextra -O2
LDLIBS  := -lm

# No Linux (não-Windows), habilita clock_gettime via POSIX
ifneq ($(OS),Windows_NT)
  CFLAGS += -D_POSIX_C_SOURCE=199309L
endif

SRC := jacobi.c utils.c main.c

.PHONY: all
all: $(EXE)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(EXE) $(LDLIBS)

.PHONY: run
run: $(EXE)
	$(RUN)

.PHONY: clean
clean:
	-$(RM) $(EXE) 2>$(NULL) || true

.PHONY: debug release
debug: CFLAGS := -Wall -Wextra -O0 -g
debug: clean all

release: CFLAGS := -Wall -Wextra -O3
release: clean all
