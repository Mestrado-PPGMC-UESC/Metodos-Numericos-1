# Nome do executável
TARGET = bissecao_exec

# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -Wextra -std=c11

# Flags de linkagem
LDFLAGS = -lm

# Arquivos fonte
SRCS = main.c bissecao.c

# Objetos
OBJS = $(SRCS:.c=.o)

# Regra principal
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c bissecao.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET) 1 1e-6 100 1

.PHONY: all clean run
