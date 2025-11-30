# Makefile para metodo da Secante

# Nome do executavel
TARGET = secante

# Compilador
CC = gcc

# Flags de compilacao
CFLAGS = -Wall -Wextra -std=c11

# Flags de linkagem (biblioteca matematica)
LDFLAGS = -lm

# Arquivos fonte
SRCS = main.c secante.c

# Objetos
OBJS = $(SRCS:.c=.o)

# Regra padrao
all: $(TARGET)

# Como gerar o executavel
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Como gerar cada .o
%.o: %.c secante.h
	$(CC) $(CFLAGS) -c $< -o $@

# Rodar o programa
run: $(TARGET)
	./$(TARGET)

# Limpar
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean run
