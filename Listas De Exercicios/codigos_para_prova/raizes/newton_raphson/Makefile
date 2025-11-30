# Makefile para Newton-Raphson

# Nome do executavel
TARGET = newton_raphson

# Compilador
CC = gcc

# Flags de compilacao
CFLAGS = -Wall -Wextra -std=c11

# Flags de linkagem (biblioteca matematica)
LDFLAGS = -lm

# Arquivos fonte
SRCS = main.c newton_raphson.c

# Objetos gerados
OBJS = $(SRCS:.c=.o)

# Regra padrao
all: $(TARGET)

# Como gerar o executavel
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Como compilar cada .c em .o
%.o: %.c newton_raphson.h
	$(CC) $(CFLAGS) -c $< -o $@

# Executar
run: $(TARGET)
	./$(TARGET)

# Limpar
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean run
