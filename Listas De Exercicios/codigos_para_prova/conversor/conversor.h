#ifndef CONVERSOR_H
#define CONVERSOR_H

#include <stddef.h>

/* Funções auxiliares e de conversão */

void limpaBufferEntrada(void);

void decimalIntParaBinario(long long n, char *out, size_t tamanho);

long long binarioParaDecimalInt(const char *bin);

double binarioFloatGenerico(const char *bin, int totalBits, int bitsExpMod);

void doubleParaBinarioFloatGenerico(double x,
                                    int totalBits,
                                    int bitsExpMod,
                                    char *out);

#endif /* CONVERSOR_H */
