# ==== Exercicio1/Metodos Iterativos/gauss-seidel-sor ====

ifeq ($(OS),Windows_NT)
  EXE  := gauss_sor.exe
  RUN  := $(EXE)
  SHELL := cmd
  RM   := del /Q
  NULL := nul
else
  EXE  := gauss_sor
  RUN  := ./$(EXE)
  RM   := rm -f
  NULL := /dev/null
endif

CC      := gcc
CFLAGS  := -Wall -Wextra -O2
LDLIBS  := -lm

# Habilita clock_gettime() no Linux
ifneq ($(OS),Windows_NT)
  CFLAGS += -D_POSIX_C_SOURCE=199309L
endif

SRC := main.c gaussseidel_sor.c utils.c

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
