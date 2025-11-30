// jacobi.h
#ifndef JACOBI_H
#define JACOBI_H

typedef enum {
    JACOBI_OK = 0,
    JACOBI_NAO_CONVERGIU = 1,
    JACOBI_SINGULAR = 2,
    JACOBI_PARAM_INVALIDO = 3
} JacobiStatus;

// Jacobi com peso (sub/over-relaxado): 0 < omega < 2
JacobiStatus jacobi_weighted(double** matrizEstendida, int ordemMatriz,
                             double* vetorSolucao, double omega,
                             double tolerancia, long long int maxIter);

// Wrapper de compatibilidade: ω = 1.0 (Jacobi clássico)
JacobiStatus jacobi(double** matrizEstendida, int ordemMatriz,
                    double* vetorSolucao, double tolerancia,
                    long long int maxIter);

void imprimirStatusJacobi(JacobiStatus status);

#endif
