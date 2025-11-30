#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "jacobi.h"

#ifndef REL_EPS_MIN
#define REL_EPS_MIN 1e-30  // evita divisão por zero na razão relativa
#endif

// ============================================================
// JACOBI (weighted): x^{k+1} = (1-ω) x^k + ω * J(x^k)
//   onde J(x^k) é a atualização Jacobi clássica
// Critério de parada: erro relativo entre ||x||_inf consecutivas < tolerancia
// Guarda simples de divergência: ||x||_inf > 1e12
// ============================================================
JacobiStatus jacobi_weighted(double** matrizEstendida, int ordemMatriz,
                             double* vetorSolucao, double omega,
                             double tolerancia, long long int maxIter)
{
    // --- validação do parâmetro ω ---
    if (!(omega > 0.0 && omega < 2.0)) {
        return JACOBI_PARAM_INVALIDO;
    }

    // --- alocar vetores de trabalho ---
    double* xAtual   = (double*) malloc(ordemMatriz * sizeof(double));
    double* xProximo = (double*) malloc(ordemMatriz * sizeof(double));
    if (!xAtual || !xProximo) {
        free(xAtual); free(xProximo);
        return JACOBI_SINGULAR; // sinaliza falha de alocação como "singular"
    }

    // --- chute inicial x^(0) = b_i / a_ii + checagem da diagonal ---
    for (int i = 0; i < ordemMatriz; i++) {
        double aii = matrizEstendida[i][i];
        if (fabs(aii) < tolerancia) {
            free(xAtual); free(xProximo);
            return JACOBI_SINGULAR; // evita divisão por ~0
        }
        xAtual[i] = matrizEstendida[i][ordemMatriz] / aii; // b_i / a_ii
    }

    // norma de x^(0)
    double normaAnt = 0.0;
    for (int i = 0; i < ordemMatriz; i++) {
        double a = fabs(xAtual[i]);
        if (a > normaAnt) normaAnt = a;
    }

    // --- iterações ---
    for (long long int k = 1; k <= maxIter; k++) {

        // J(x^k): atualização Jacobi clássica (em xProximo como rascunho)
        for (int i = 0; i < ordemMatriz; i++) {
            double soma = matrizEstendida[i][ordemMatriz]; // b_i
            for (int j = 0; j < ordemMatriz; j++) {
                if (j == i) continue;
                soma -= matrizEstendida[i][j] * xAtual[j];
            }
            double xjac = soma / matrizEstendida[i][i];

            // Weighted Jacobi: mistura com xAtual
            xProximo[i] = (1.0 - omega) * xAtual[i] + omega * xjac;
        }

        // norma de x^{k+1}
        double normaAtual = 0.0;
        for (int i = 0; i < ordemMatriz; i++) {
            double a = fabs(xProximo[i]);
            if (a > normaAtual) normaAtual = a;
        }

        // erro relativo
        double denom = fmax(fmax(normaAtual, normaAnt), REL_EPS_MIN);
        double rel_err = fabs(normaAtual - normaAnt) / denom;

        if (rel_err < tolerancia) {
            // copia solução e reporta #iterações (compatível com seu GS)
            for (int i = 0; i < ordemMatriz; i++) {
                vetorSolucao[i] = xProximo[i];
            }
            printf("%lld interacoes\n", k - 1);
            free(xAtual); free(xProximo);
            return JACOBI_OK;
        }

        // guarda de divergência simples
        if (normaAtual > 1e12) {
            // devolve último x conhecido (xAtual)
            for (int i = 0; i < ordemMatriz; i++) {
                vetorSolucao[i] = xAtual[i];
            }
            free(xAtual); free(xProximo);
            return JACOBI_NAO_CONVERGIU;
        }

        // prepara próxima iteração
        for (int i = 0; i < ordemMatriz; i++) {
            xAtual[i] = xProximo[i];
        }
        normaAnt = normaAtual;
    }
    // --- não convergiu em maxIter; devolve último x ---
    for (int i = 0; i < ordemMatriz; i++) vetorSolucao[i] = xAtual[i];
    free(xAtual); free(xProximo);
    return JACOBI_NAO_CONVERGIU;
}

// Wrapper: Jacobi clássico (ω = 1.0)
JacobiStatus jacobi(double** matrizEstendida, int ordemMatriz,
                    double* vetorSolucao, double tolerancia, long long int maxIter)
{
    return jacobi_weighted(matrizEstendida, ordemMatriz, vetorSolucao,
                           1.0, tolerancia, maxIter);
}

// Status
void imprimirStatusJacobi(JacobiStatus status) {
    if (status == JACOBI_OK) {
        puts("Jacobi: convergiu.");
    } else if (status == JACOBI_NAO_CONVERGIU) {
        puts("Jacobi: não convergiu dentro do número máximo de iterações.");
    } else if (status == JACOBI_SINGULAR) {
        puts("Jacobi: sistema singular ou diagonal nula.");
    } else { // JACOBI_PARAM_INVALIDO
        puts("Jacobi: parâmetro omega inválido (use 0 < omega < 2).");
    }
}
