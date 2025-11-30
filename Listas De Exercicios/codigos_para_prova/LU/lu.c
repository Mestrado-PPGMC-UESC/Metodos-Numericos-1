#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "lu.h"
#include "utils.h"

/* Flag interna de aviso (pivô ~ 0) */
static int g_flagPivoQuaseZero = 0;



/**
 * Fatoração LU por multiplicadores (sem pivotamento).
 * A = L * U, onde:
 *  - U é obtida fazendo eliminações em A (escalonando linhas)
 *  - L guarda os multiplicadores abaixo da diagonal e 1 na diagonal
 */
LUStatus luDecomposicao(double** matrizA,
                        int ordemMatriz,
                        double** matrizL,
                        double** matrizU,
                        double tolerancia)
{
    // (1) Inicializar U como cópia de A e L como identidade
    for (int linha = 0; linha < ordemMatriz; linha++) {
        for (int coluna = 0; coluna < ordemMatriz; coluna++) {
            matrizU[linha][coluna] = matrizA[linha][coluna];          // U <- A
            matrizL[linha][coluna] = (linha == coluna) ? 1.0 : 0.0;   // L <- I
        }
    }

    // (2) Loop de pivôs: elimina abaixo da diagonal, coluna por coluna
    for (int indicePivo = 0; indicePivo < ordemMatriz - 1; indicePivo++) {
        double valorPivo = matrizU[indicePivo][indicePivo];

        // Aviso (sem abortar)
        if (fabs(valorPivo) < tolerancia) {
            g_flagPivoQuaseZero = 1;
            // segue o cálculo mesmo assim
        }

        // (2.1) Para cada linha abaixo do pivô, zere a entrada na coluna do pivô
        for (int linhaEliminada = indicePivo + 1; linhaEliminada < ordemMatriz; linhaEliminada++) {
            double multiplicador = matrizU[linhaEliminada][indicePivo] / valorPivo;
            matrizL[linhaEliminada][indicePivo] = multiplicador; // guarda multiplicador em L

            // Atualiza toda a linha (inclusive a coluna do pivô, que vira 0)
            for (int colunaAtual = indicePivo; colunaAtual < ordemMatriz; colunaAtual++) {
                double antes = matrizU[linhaEliminada][colunaAtual];
                double termo = multiplicador * matrizU[indicePivo][colunaAtual];
                matrizU[linhaEliminada][colunaAtual] = antes - termo;
            }
        }
    }

    return LU_OK;
}

int substituicaoProgressiva(double** matrizL, const double* vetorB,
                            double* vetorY, int ordemMatriz, double tolerancia)
{
    for (int i = 0; i < ordemMatriz; i++) {
        double soma = 0.0;
        for (int j = 0; j < i; j++) soma += matrizL[i][j] * vetorY[j];

        double diagonal = matrizL[i][i]; // em Doolittle diag(L)=1, mas deixo geral
        if (fabs(diagonal) < tolerancia) {
            g_flagPivoQuaseZero = 1; // avisa e segue
        }
        vetorY[i] = (vetorB[i] - soma) / diagonal;
    }
    return 0;
}

int substituicaoRegressiva_U(double** matrizU, const double* vetorY,
                             double* vetorX, int ordemMatriz, double tolerancia)
{
    for (int i = ordemMatriz - 1; i >= 0; i--) {
        double soma = 0.0;
        for (int j = i + 1; j < ordemMatriz; j++) soma += matrizU[i][j] * vetorX[j];

        double diagonal = matrizU[i][i];
        if (fabs(diagonal) < tolerancia) {
            g_flagPivoQuaseZero = 1; // avisa e segue
        }
        vetorX[i] = (vetorY[i] - soma) / diagonal;
    }
    return 0;
}

int inversaAPartirDeLU(double** matrizL, double** matrizU,
                       double** matrizInversa, int ordemMatriz, double tolerancia)
{
    // para cada coluna j da identidade, resolvemos Ly=e_j e depois Ux=y
    double* vetorB = (double*) malloc(ordemMatriz * sizeof(double));
    double* vetorY = (double*) malloc(ordemMatriz * sizeof(double));
    double* vetorX = (double*) malloc(ordemMatriz * sizeof(double));
    if (!vetorB || !vetorY || !vetorX) { free(vetorB); free(vetorY); free(vetorX); return 1; }

    for (int j = 0; j < ordemMatriz; j++) {
        // e_j
        for (int i = 0; i < ordemMatriz; i++) vetorB[i] = (i == j) ? 1.0 : 0.0;

        substituicaoProgressiva(matrizL, vetorB, vetorY, ordemMatriz, tolerancia);
        substituicaoRegressiva_U(matrizU, vetorY, vetorX, ordemMatriz, tolerancia);

        // grava vetorX como coluna j da inversa
        for (int i = 0; i < ordemMatriz; i++) matrizInversa[i][j] = vetorX[i];
    }

    free(vetorB); free(vetorY); free(vetorX);
    return 0;
}

int inversaPorLU(double** matrizA, int ordemMatriz, double tolerancia,
                 double** matrizInversa)
{
    // 1) fatorar A = L U
    double** matrizL = alocarMatriz(ordemMatriz);
    double** matrizU = alocarMatriz(ordemMatriz);

    // zera avisos desta rodada
    luRedefinirAvisos();

    luDecomposicao(matrizA, ordemMatriz, matrizL, matrizU, tolerancia); // não aborta
    // 2) montar inversa a partir de L e U
    int ok = inversaAPartirDeLU(matrizL, matrizU, matrizInversa, ordemMatriz, tolerancia);

    liberarMatriz(matrizL, ordemMatriz);
    liberarMatriz(matrizU, ordemMatriz);
    return ok; // 0 = executou; use luFlagPivoQuaseZero() para saber se é instável
}


void luRedefinirAvisos(){
     g_flagPivoQuaseZero = 0;
}


int  luFlagPivoQuaseZero(){ 
    return g_flagPivoQuaseZero;
}