# ==== Projeto: Exercicio2/LU ====

ifeq ($(OS),Windows_NT)
  EXEEXT := .exe
  RM := del /Q
  NULL := nul
else
  EXEEXT :=
  RM := rm -f
  NULL := /dev/null
endif

CC      := gcc
CFLAGS  := -Wall -Wextra -O2
LDLIBS  := -lm

SRC := $(wildcard *.c)
OBJ := $(SRC:.c=.o)
BIN := lu$(EXEEXT)

.PHONY: all
all: $(BIN)
	@$(RM) $(OBJ) 2>$(NULL) || true  # apaga os .o automaticamente após compilar

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: run
run: $(BIN)
	./$(BIN)

.PHONY: clean
clean:
	$(RM) $(OBJ) $(BIN) 2>$(NULL) || true

.PHONY: debug release
debug:
	$(MAKE) clean
	$(CC) -Wall -Wextra -O0 -g $(SRC) -o $(BIN) $(LDLIBS)

release:
	$(MAKE) clean
	$(CC) -Wall -Wextra -O3 $(SRC) -o $(BIN) $(LDLIBS)
