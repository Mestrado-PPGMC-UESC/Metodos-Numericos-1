# Makefile para ponto_fixo

# Nome do executável
TARGET = ponto_fixo

# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -Wextra -std=c11

# Flags de linkagem (biblioteca matemática)
LDFLAGS = -lm

# Arquivos fonte
SRCS = main.c ponto_fixo.c

# Objetos gerados
OBJS = $(SRCS:.c=.o)

# Regra padrão: compilar tudo
all: $(TARGET)

# Como gerar o executável
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Como compilar cada .c em .o
%.o: %.c ponto_fixo.h
	$(CC) $(CFLAGS) -c $< -o $@

# Regra de execução com parâmetros de exemplo
run: $(TARGET)
	./$(TARGET) 0.5 5 50 1

# Limpar objetos e binário
clean:
	rm -f $(OBJS) $(TARGET)

# Evita que "make clean" / "make run" sejam interpretados como arquivos
.PHONY: all clean run
