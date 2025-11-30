// main_jacobi_relaxado.c
#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>

#include "utils.h"   // lerMatrizAumentadaDeArquivo, criarVetorSolucao, imprimirSolucao, liberar, calcularErroRelativo
#include "jacobi.h"  // jacobi_weighted, JacobiStatus, imprimirStatusJacobi

// Função auxiliar para medir tempo
static double diffTempo(const struct timespec* inicio,
                        const struct timespec* fim) {
    long seg  = fim->tv_sec  - inicio->tv_sec;
    long nsec = fim->tv_nsec - inicio->tv_nsec;
    return (double)seg + (double)nsec / 1e9;
}

int main(void) {
    double tolerancia  = 1e-12;
    long long int maxIter = LLONG_MAX;

    // ============================================
    // Loop sobre valores de omega (sub-relaxamento)
    // Ex.: 0.1, 0.2, ..., 0.9
    // ============================================
    for (double omega = 0.1; omega <= 0.9 + 1e-9; omega += 0.1) {
        printf("\n============================\n");
        printf("Jacobi (weighted) com omega = %.2f\n", omega);

        // ----------------------------------------
        // Ler sistema do arquivo
        // ----------------------------------------
        int ordemMatriz = 0;
        double** matrizEstendida =
            lerMatrizAumentadaDeArquivo("matriz.txt", &ordemMatriz);

        double* vetorSolucao = criarVetorSolucao(ordemMatriz);

        // ----------------------------------------
        // Medir tempo
        // ----------------------------------------
        struct timespec inicio, fim;
        clock_gettime(CLOCK_MONOTONIC, &inicio);

        JacobiStatus status = jacobi_weighted(
            matrizEstendida, ordemMatriz, vetorSolucao,
            omega, tolerancia, maxIter
        );

        clock_gettime(CLOCK_MONOTONIC, &fim);
        double tempoSegundos = diffTempo(&inicio, &fim);

        // ----------------------------------------
        // Imprimir resultado
        // ----------------------------------------
        imprimirStatusJacobi(status);
        if (status == JACOBI_OK) {
            imprimirSolucao(vetorSolucao, ordemMatriz);
            // solução exata presumida: (1, ..., 1)
            calcularErroRelativo(vetorSolucao, ordemMatriz);
        } else {
            printf("[ERRO] Jacobi weighted nao convergiu para omega = %.2f\n", omega);
        }

        printf("Tempo de execucao: %.10f segundos\n", tempoSegundos);

        // ----------------------------------------
        // Liberar memória
        // ----------------------------------------
        liberar(matrizEstendida, ordemMatriz, vetorSolucao);
    }

    return 0;
}
