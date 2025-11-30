#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>

#include "utils.h"          // criarVetorSolucao, imprimirSolucao, liberar, calcularErroRelativo
#include "gaussseidel.h"    // gaussSeidel, GaussSeidelStatus, imprimirStatusGaussSeidel

// Função para diferença de tempo
static double diffTempo(const struct timespec* inicio,
                        const struct timespec* fim) {
    long seg  = fim->tv_sec  - inicio->tv_sec;
    long nsec = fim->tv_nsec - inicio->tv_nsec;
    return (double)seg + (double)nsec / 1e9;
}

int main(void) {
    // ============================================
    // ETAPA 1: Ler sistema do arquivo
    // ============================================
    int ordemMatriz = 0;
    double** matrizEstendida = lerMatrizAumentadaDeArquivo("matriz.txt", &ordemMatriz);

    // ============================================
    // ETAPA 2: Preparar vetor solução
    // ============================================
    double* vetorSolucao = criarVetorSolucao(ordemMatriz);

    // ============================================
    // ETAPA 3: Configurar parâmetros do Gauss–Seidel
    // ============================================
    double tolerancia     = 1e-12;
    long long int maxIter = LLONG_MAX;   // Pode alterar depois

    // ============================================
    // ETAPA 4: Medir tempo de execução
    // ============================================
    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    GaussSeidelStatus status =
        gaussSeidel(matrizEstendida, ordemMatriz, vetorSolucao,
                    tolerancia, maxIter);

    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempoSegundos = diffTempo(&inicio, &fim);

    // ============================================
    // ETAPA 5: Mostrar resultado
    // ============================================
    imprimirStatusGaussSeidel(status);

    if (status == GS_OK) {
        imprimirSolucao(vetorSolucao, ordemMatriz);
        // calcularErroRelativo(vetorSolucao, ordemMatriz);   // <-- Usando a função antiga
    } else {
        puts("[ERRO] Gauss–Seidel não convergiu.");
    }

    printf("\nTempo de execucao: %.10f segundos\n", tempoSegundos);

    // ============================================
    // ETAPA 6: Liberar memória
    // ============================================
    liberar(matrizEstendida, ordemMatriz, vetorSolucao);

    // ============================================
    // ETAPA 7: Encerrar com código de status
    // ============================================
    return (status == GS_OK) ? 0 : 1;
}
