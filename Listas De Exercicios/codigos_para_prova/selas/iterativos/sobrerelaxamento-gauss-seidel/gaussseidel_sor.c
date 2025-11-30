#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gaussseidel_sor.h"

#ifndef REL_EPS_MIN
#define REL_EPS_MIN 1e-30  // evita divisão por zero no cálculo de erro relativo
#endif

// ============================================================
// Gauss–Seidel com Sobre-Relaxamento (SOR)
// ============================================================
GaussSeidelSORStatus gaussSeidelSOR(double** matrizEstendida, int ordemMatriz,
                                    double* vetorSolucao, double omega,
                                    double tolerancia, long long int maxIter) {
    // --- Etapa 0: validar parâmetros ---
    if (!(omega > 0.0 && omega < 2.0)) {
        return SOR_PARAM_INVALIDO;
    }

    // --- Etapa 1: chute inicial x^(0) = b_i / a_ii (e checagem da diagonal) ---
    for (int i = 0; i < ordemMatriz; i++) {
        double aii = matrizEstendida[i][i];
        if (fabs(aii) < tolerancia) {
            return SOR_SINGULAR; // pivô inválido
        }
        vetorSolucao[i] = matrizEstendida[i][ordemMatriz] / aii;
    }

    // norma de x^(0)
    double normaAnt = 0.0;
    for (int i = 0; i < ordemMatriz; i++) {
        double a = fabs(vetorSolucao[i]);
        if (a > normaAnt) normaAnt = a;
    }

    // --- Etapa 2: iterações SOR ---
    for (long long int  k = 1; k <= maxIter; k++) {
        // varre componentes i = 0..n-1 (atualização imediata)
        for (int i = 0; i < ordemMatriz; i++) {
            double soma = matrizEstendida[i][ordemMatriz]; // b_i

            // usa x_j atualizado se j < i; x_j antigo se j > i
            for (int j = 0; j < ordemMatriz; j++) {
                if (j == i) continue;
                soma -= matrizEstendida[i][j] * vetorSolucao[j];
            }

            // valor estilo Gauss–Seidel (sem relaxamento)
            double x_gs = soma / matrizEstendida[i][i];

            // SOR: combinação de x^k e x^{GS}
            vetorSolucao[i] = (1.0 - omega) * vetorSolucao[i] + omega * x_gs;
        }

        // norma de x^(k)
        double normaAtual = 0.0;
        for (int i = 0; i < ordemMatriz; i++) {
            double a = fabs(vetorSolucao[i]);
            if (a > normaAtual) normaAtual = a;
        }

        // erro relativo entre normas consecutivas
        double denom = fmax(fmax(normaAtual, normaAnt), REL_EPS_MIN);
        double rel_err = fabs(normaAtual - normaAnt) / denom;

        // (opcional) imprimir progresso: descomente se quiser acompanhar
        // printf("Iteracao %d: ||x||_inf = %.6e | erro_rel = %.6e\n", k, normaAtual, rel_err);

        if (rel_err < tolerancia) {
            printf("%lld interacoes\n",k-1 );
            return SOR_OK;
        }

        normaAnt = normaAtual;

        // guarda de divergência simples (opcional)
        if (normaAtual > 1e12) {
            return SOR_NAO_CONVERGIU;
        }
    }

    // --- Etapa 3: não convergiu dentro de maxIter ---
    return SOR_NAO_CONVERGIU;
}

// ============================================================
// Impressão do status
// ============================================================
void imprimirStatusGaussSeidelSOR(GaussSeidelSORStatus status) {
    if (status == SOR_OK) {
        puts("SOR: convergiu.");
    } else if (status == SOR_NAO_CONVERGIU) {
        puts("SOR: não convergiu dentro do número máximo de iterações.");
    } else if (status == SOR_SINGULAR) {
        puts("SOR: sistema singular ou pivô nulo.");
    } else { // SOR_PARAM_INVALIDO
        puts("SOR: parâmetro omega inválido (use 0 < omega < 2).");
    }
}
