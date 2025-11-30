#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gaussseidel.h"

// ============================================================
// Método de Gauss-Seidel
// ============================================================
GaussSeidelStatus gaussSeidel(double** matrizEstendida, int ordemMatriz,
                              double* vetorSolucao, double tolerancia, long long int maxIter) {
    // --- Etapa 1: chute inicial (x^(0) = b_i / a_ii) ---
    for (int i = 0; i < ordemMatriz; i++) {
        double aii = matrizEstendida[i][i];
        if (fabs(aii) < tolerancia) {
            return GS_SINGULAR; // pivô inválido
        }
        vetorSolucao[i] = matrizEstendida[i][ordemMatriz] / aii;
    }

    // --- Etapa 2: iterações de Gauss-Seidel ---
    for (long long int k = 0; k < maxIter; k++) {
        double normaInf = 0.0;

        for (int i = 0; i < ordemMatriz; i++) {
            double soma = matrizEstendida[i][ordemMatriz]; // b_i

            // usa x[j] atualizado se j < i, e antigo se j > i
            for (int j = 0; j < ordemMatriz; j++) {
                if (j == i) continue;
                soma -= matrizEstendida[i][j] * vetorSolucao[j];
            }

            double novoXi = soma / matrizEstendida[i][i];

            double diff = fabs(novoXi - vetorSolucao[i]);
            if (diff > normaInf) normaInf = diff;

            vetorSolucao[i] = novoXi; // atualização imediata
        }

        // critério de parada relativo
        if (normaInf < tolerancia) {
            printf("%lld interacoes\n",k );
            return GS_OK;
        }
    }

    return GS_NAO_CONVERGIU;
}

// ============================================================
// Impressão do status
// ============================================================
void imprimirStatusGaussSeidel(GaussSeidelStatus status) {
    if (status == GS_OK) {
        puts("Gauss-Seidel: convergiu.");
    } else if (status == GS_NAO_CONVERGIU) {
        puts("Gauss-Seidel: não convergiu dentro do número máximo de iterações.");
    } else {
        puts("Gauss-Seidel: sistema singular ou pivô nulo.");
    }
}
