#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>

#include "utils.h"              // lerMatrizAumentadaDeArquivo, criarVetorSolucao, imprimirSolucao, liberar
#include "gaussseidel_sor.h"    // gaussSeidelSOR, imprimirStatusGaussSeidelSOR

// Função auxiliar para medir tempo
static double diffTempo(const struct timespec* inicio,
                        const struct timespec* fim) {
    long seg  = fim->tv_sec  - inicio->tv_sec;
    long nsec = fim->tv_nsec - inicio->tv_nsec;
    return (double)seg + (double)nsec / 1e9;
}

int main(void) {

    double tolerancia = 1e-12;
    long long int maxIter = LLONG_MAX;

    // ==============================================================
    // Vamos testar vários valores de omega
    // ==============================================================

    for (double omega = 1.1; omega <= 2.0 + 1e-9; omega += 0.1) {

        printf("\n=====================================\n");
        printf("Teste com omega = %.1f\n", omega);

        // ============================================
        // Ler matriz do arquivo (tem que recriar a cada iteração)
        // ============================================
        int ordemMatriz = 0;
        double** matrizEstendida =
            lerMatrizAumentadaDeArquivo("matriz.txt", &ordemMatriz);

        double* vetorSolucao = criarVetorSolucao(ordemMatriz);

        // ============================================
        // Medir tempo
        // ============================================
        struct timespec inicio, fim;
        clock_gettime(CLOCK_MONOTONIC, &inicio);

        GaussSeidelSORStatus status =
            gaussSeidelSOR(matrizEstendida, ordemMatriz,
                           vetorSolucao, omega,
                           tolerancia, maxIter);

        clock_gettime(CLOCK_MONOTONIC, &fim);
        double tempoSegundos = diffTempo(&inicio, &fim);

        // ============================================
        // Exibir resultados
        // ============================================
        imprimirStatusGaussSeidelSOR(status);

        if (status == SOR_OK) {
            imprimirSolucao(vetorSolucao, ordemMatriz);

            // Erro relativo assumindo solução exata (1, ..., 1)
            calcularErroRelativo(vetorSolucao, ordemMatriz);
        } else {
            printf("[ERRO] SOR não convergiu para ω = %.2f\n", omega);
        }

        printf("Tempo de execucao: %.10f segundos\n", tempoSegundos);

        // ============================================
        // Liberar memória
        // ============================================
        liberar(matrizEstendida, ordemMatriz, vetorSolucao);
    }

    return 0;
}
