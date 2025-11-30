#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "jacobi.h"

#ifndef REL_EPS_MIN
#define REL_EPS_MIN 1e-30  // evita divisão por zero na razão relativa
#endif

// ============================================================
// JACOBI — método iterativo (x^(0) = b_i / a_ii)
// Critério de parada: erro relativo entre ||x||_inf das iterações
// consecutivas menor que 'tolerancia'.
// ============================================================
JacobiStatus jacobi(double** matrizEstendida, int ordemMatriz,
                    double* vetorSolucao, double tolerancia, long long int maxIter) {
    // --- Etapa 1: alocar vetores de trabalho ---
    double* xAtual   = (double*) malloc(ordemMatriz * sizeof(double));
    double* xProximo = (double*) malloc(ordemMatriz * sizeof(double));
    if (!xAtual || !xProximo) {
        free(xAtual); free(xProximo);
        return JACOBI_SINGULAR;
    }

    // --- Etapa 2: checar diagonal e definir chute inicial x^(0) = b_i / a_ii ---
    for (int i = 0; i < ordemMatriz; i++) {
        double aii = matrizEstendida[i][i];
        // if (fabs(aii) < tolerancia) {
        //     free(xAtual); free(xProximo);
        //     return JACOBI_SINGULAR; // evita divisão por ~0
        // }
        xAtual[i] = matrizEstendida[i][ordemMatriz] / aii; // b_i / a_ii
    }


    // --- Etapa 3: iterações do método de Jacobi ---
    double normaAnt = 0.0;
    // norma do xAtual (k = 0)
    for (int i = 0; i < ordemMatriz; i++) {
        double a = fabs(xAtual[i]);
        if (a > normaAnt) normaAnt = a;
    }

    for (long long int k = 1; k <= maxIter; k++) {
        // monta xProximo
        for (int i = 0; i < ordemMatriz; i++) {
            double soma = matrizEstendida[i][ordemMatriz]; // b_i
            for (int j = 0; j < ordemMatriz; j++) {
                if (j == i) continue;
                soma -= matrizEstendida[i][j] * xAtual[j];
            }
            xProximo[i] = soma / matrizEstendida[i][i];
        }

        // normas infinito (||x_k|| e ||x_{k+1}||)
        double normaAtual = 0.0;
        for (int i = 0; i < ordemMatriz; i++) {
            double a = fabs(xProximo[i]);
            if (a > normaAtual) normaAtual = a;
        }

        // erro relativo baseado nas normas consecutivas
        double denom = fmax(fmax(normaAtual, normaAnt), REL_EPS_MIN);
        double rel_err = fabs(normaAtual - normaAnt) / denom;



        // critério de parada relativo
        if (rel_err < tolerancia) {
            for (int i = 0; i < ordemMatriz; i++){
                vetorSolucao[i] = xProximo[i];
            } 
            free(xAtual); free(xProximo);
            printf("%lld iteracores \n",k);
            return JACOBI_OK;
        }


        // prepara próxima iteração: x^{k} <- x^{k+1}
        for (int i = 0; i < ordemMatriz; i++){
            xAtual[i] = xProximo[i];

        } 
        normaAnt = normaAtual;
    }

    // --- Etapa 4: não convergiu em maxIter; devolve último x aproximado ---
    for (int i = 0; i < ordemMatriz; i++) vetorSolucao[i] = xAtual[i];
    free(xAtual); free(xProximo);
    return JACOBI_NAO_CONVERGIU;
}

// ============================================================
// Utilitário: imprimir status do Jacobi
// ============================================================
void imprimirStatusJacobi(JacobiStatus status) {
    if (status == JACOBI_OK) {
        puts("Jacobi: convergiu.");
    } else if (status == JACOBI_NAO_CONVERGIU) {
        puts("Jacobi: não convergiu dentro do número máximo de iterações.");
    } else { // JACOBI_SINGULAR
        puts("Jacobi: sistema singular ou diagonal nula.");
    }
}
