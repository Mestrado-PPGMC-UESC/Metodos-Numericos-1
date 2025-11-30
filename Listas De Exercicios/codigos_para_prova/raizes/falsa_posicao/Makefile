# Nome do executável
TARGET = falsa_posicao

# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -Wextra -std=c11

# Flags de linkagem (biblioteca matemática)
LDFLAGS = -lm

# Arquivos fonte
SRCS = main.c falsa_posicao.c

# Arquivos objeto (.o)
OBJS = $(SRCS:.c=.o)

# Regra padrão (compila tudo)
all: $(TARGET)

# Linkagem final
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compilação de cada .c → .o
%.o: %.c falsa_posicao.h
	$(CC) $(CFLAGS) -c $< -o $@

# Executar automaticamente
run: $(TARGET)
	./$(TARGET)

# Remover binários e objetos
clean:
	rm -f $(OBJS) $(TARGET)

# Evitar conflito com arquivos
.PHONY: all clean run
