# Nome do executável
TARGET = conversor

# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -Wextra -std=c11

# Flags de linkagem (biblioteca matemática)
LDFLAGS = -lm

# Arquivos fonte
SRCS = main.c conversor.c

# Objetos gerados
OBJS = $(SRCS:.c=.o)

# Regra padrão: compilar tudo
all: $(TARGET)

# Como gerar o executável
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Como compilar cada .c em .o
%.o: %.c conversor.h
	$(CC) $(CFLAGS) -c $< -o $@

# Executar o programa
run: $(TARGET)
	./$(TARGET)

# Limpar objetos e binário
clean:
	rm -f $(OBJS) $(TARGET)

# Evita que "make clean" seja interpretado como arquivo
.PHONY: all clean run
