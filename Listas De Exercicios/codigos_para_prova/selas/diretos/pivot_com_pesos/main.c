#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>        // clock_gettime

#include "utils.h"       // lerMatrizAumentadaDeArquivo, criarVetorSolucao,
                         // imprimirSolucao, liberar, processarSolucaoExata
#include "gauss.h"       // gauss, GaussStatus, imprimirStatus, gaussFlagPivoQuaseZero

// Calcula diferença de tempo (fim - inicio), em segundos
static double diffTempo(const struct timespec* inicio,
                        const struct timespec* fim) {
    long seg  = fim->tv_sec  - inicio->tv_sec;
    long nsec = fim->tv_nsec - inicio->tv_nsec;
    return (double)seg + (double)nsec / 1e9;
}

/**
 * @brief Programa principal para resolução de sistemas lineares via Método de Gauss
 *        com pivotamento escalonado (com pesos) — FAIL-SOFT.
 *
 * Lê de um arquivo texto uma matriz aumentada [A|b],
 * mede o tempo de execução, imprime status/avisos e,
 * opcionalmente, compara a solução numérica com uma solução exata
 * fornecida em outro arquivo.
 */
int main(void) {
    // ============================================
    // ETAPA 1: Ler sistema linear de teste do arquivo
    // ============================================
    int ordemMatriz = 0;

    // Lê [A|b] e descobre n a partir do arquivo
    double** matrizEstendida = lerMatrizAumentadaDeArquivo("matriz.txt", &ordemMatriz);
    double* vetorSolucao     = criarVetorSolucao(ordemMatriz);
    const double tolerancia  = 1e-12;

    // ============================================
    // ETAPA 2: Medir tempo de execução (Linux)
    // ============================================
    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    // Método de Gauss com pivotamento escalonado (com pesos)
    GaussStatus status = gauss(matrizEstendida, ordemMatriz, vetorSolucao, tolerancia);

    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempoSegundos = diffTempo(&inicio, &fim);
    double tempoNanoSeg  = tempoSegundos * 1e9;

    // ============================================
    // ETAPA 3: Exibir resultados
    // ============================================
    imprimirStatus(status);

    if (status == GAUSS_INCONSISTENTE) {
        puts("[AVISO] Sistema inconsistente detectado: nao ha solucao (linha nula em A com b != 0).");
    } else if (status == GAUSS_SINGULAR || gaussFlagPivoQuaseZero()) {
        puts("[AVISO] Sistema singular ou numericamente instavel (pivo ≈ 0). Resultados podem ser imprecisos.");
    }

    // Exibe solução sempre (fail-soft)
    imprimirSolucao(vetorSolucao, ordemMatriz);

    // =======================================================
    // BLOCO OPCIONAL — comparação com solução exata
    // Para desativar, basta comentar a linha abaixo:
    processarSolucaoExata("solucao.txt", vetorSolucao, ordemMatriz);
    // =======================================================

    printf("\nTempo de execucao: %.9f segundos (%.0f ns)\n",
           tempoSegundos, tempoNanoSeg);

    // ============================================
    // ETAPA 4: Liberar memória
    // ============================================
    liberar(matrizEstendida, ordemMatriz, vetorSolucao);

    // ============================================
    // ETAPA 5: Encerrar com código de status
    // ============================================
    if (status == GAUSS_OK)           return 0;
    if (status == GAUSS_SINGULAR)     return 1;
    if (status == GAUSS_INCONSISTENTE) return 2;
    return 0;
}
