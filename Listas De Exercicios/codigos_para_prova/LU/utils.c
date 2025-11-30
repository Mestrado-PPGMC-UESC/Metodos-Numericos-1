#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"

double** alocarMatriz(int ordemMatriz) {
    double** matriz = (double**)malloc(ordemMatriz * sizeof(double*));
    for (int linha = 0; linha < ordemMatriz; linha++) {
        matriz[linha] = (double*)malloc(ordemMatriz * sizeof(double));
    }
    return matriz;
}



void liberarMatriz(double** matriz, int ordemMatriz) {
    for (int linha = 0; linha < ordemMatriz; linha++) {
        free(matriz[linha]);
    }
    free(matriz);
}

void imprimirMatriz(const char* titulo, double** matriz, int ordemMatriz) {
    puts("");

    puts(titulo);
    for (int linha = 0; linha < ordemMatriz; linha++) {
        for (int coluna = 0; coluna < ordemMatriz; coluna++) {
            printf("%.16e ", matriz[linha][coluna]);
        }
        puts("");
    }
    puts("");
}

void copiarMatriz(double** destino, double** origem, int ordemMatriz) {
    for (int linha = 0; linha < ordemMatriz; linha++) {
        for (int coluna = 0; coluna < ordemMatriz; coluna++) {
            destino[linha][coluna] = origem[linha][coluna];
        }
    }
}

void multiplicarMatrizes(double** matrizA, double** matrizB, double** matrizResultado, int ordemMatriz) {
    for (int linha = 0; linha < ordemMatriz; linha++) {
        for (int coluna = 0; coluna < ordemMatriz; coluna++) {
            double soma = 0.0L;
            for (int k = 0; k < ordemMatriz; k++) {
                soma += matrizA[linha][k] * matrizB[k][coluna];
            }
            matrizResultado[linha][coluna] = soma;

        }
    }
}

// Preenche "matrizErros" com |M - I|
void calcularErrosIdentidade(double** matriz, int ordemMatriz, double** matrizErros) {
    for (int linha = 0; linha < ordemMatriz; linha++) {
        for (int coluna = 0; coluna < ordemMatriz; coluna++) {
            double valorEsperado = (linha == coluna) ? 1.0 : 0.0;
            matrizErros[linha][coluna] = fabs(matriz[linha][coluna] - valorEsperado);
        }
    }
}

// Variante que também retorna o erro máximo (útil para um resumo rápido)
double calcularErrosIdentidadeComMax(double** matriz, int ordemMatriz, double** matrizErros) {
    double erroMaximo = 0.0;
    for (int linha = 0; linha < ordemMatriz; linha++) {
        for (int coluna = 0; coluna < ordemMatriz; coluna++) {
            double valorEsperado = (linha == coluna) ? 1.0 : 0.0;
            double erroAtual = fabs(matriz[linha][coluna] - valorEsperado);
            matrizErros[linha][coluna] = erroAtual;
            if (erroAtual > erroMaximo) erroMaximo = erroAtual;
        }
    }
    return erroMaximo;
}



double** clonarMatriz(double** matrizOriginal, int tamanhoMatriz) {
    double** matrizClone = alocarMatriz(tamanhoMatriz);

    for (int linha = 0; linha < tamanhoMatriz; linha++) {
        for (int coluna = 0; coluna < tamanhoMatriz; coluna++) {
            matrizClone[linha][coluna] = matrizOriginal[linha][coluna];
        }
    }

    return matrizClone;
}
// =====================
// Matrizes do Exercício 2
// =====================
double** criarMatrizA1() {
    int n = 3;
    double** A = alocarMatriz(n);
    A[0][0] = 10;  A[0][1] =  2;  A[0][2] = -1;
    A[1][0] = -3;  A[1][1] = -6;  A[1][2] =  2;
    A[2][0] =  1;  A[2][1] =  1;  A[2][2] =  5;
    return A;
}

double** criarMatrizA2() {
    int n = 4;
    double** A = alocarMatriz(n);
    A[0][0] =  1;  A[0][1] =  4;  A[0][2] =  9;  A[0][3] = 16;
    A[1][0] =  4;  A[1][1] =  9;  A[1][2] = 16;  A[1][3] = 25;
    A[2][0] =  9;  A[2][1] = 16;  A[2][2] = 25;  A[2][3] = 36;
    A[3][0] = 16;  A[3][1] = 25;  A[3][2] = 36;  A[3][3] = 49;
    return A;
}
