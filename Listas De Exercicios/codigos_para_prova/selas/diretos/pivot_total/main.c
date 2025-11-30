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
 *        com pivotamento total — FAIL-SOFT.
 *
 * Lê de um arquivo texto uma matriz aumentada [A|b],
 * resolve o sistema, mede o tempo de execução e, opcionalmente,
 * compara a solução numérica com uma solução exata fornecida em outro arquivo.
 */
int main(void) {
    // ============================================
    // ETAPA 1: Ler sistema linear de teste do arquivo
    // ============================================
    int ordemMatriz = 0;

    // Lê [A|b] e descobre n a partir do arquivo
    double** matrizEstendida = lerMatrizAumentadaDeArquivo("matriz.txt", &ordemMatriz);

    // Vetor solução (x) será preenchido após a resolução
    double* vetorSolucao = criarVetorSolucao(ordemMatriz);

    // Tolerância numérica para detectar pivôs ~ 0
    const double tolerancia = 1e-12;

    // ============================================
    // ETAPA 2: Medir tempo de execução (Linux)
    // ============================================
    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    // Aqui, gauss deve ser a versão com pivotamento total
    GaussStatus status = gauss(matrizEstendida, ordemMatriz, vetorSolucao, tolerancia);

    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempoSegundos = diffTempo(&inicio, &fim);
    double tempoNanoSeg  = tempoSegundos * 1e9;

    // ============================================
    // ETAPA 3: Exibir resultados
    // ============================================
    if (gaussFlagPivoQuaseZero()) {
        puts("[AVISO] Pivo muito pequeno detectado (possivel singularidade/instabilidade numerica).");
    }

    imprimirStatus(status);

    if (status == GAUSS_INCONSISTENTE) {
        puts("[ERRO] Sistema inconsistente: nao ha solucao (linha nula em A com b != 0).");
    } else {
        // Exibe solução mesmo que singular/instável (fail-soft)
        imprimirSolucao(vetorSolucao, ordemMatriz);

        // =======================================================
        // BLOCO OPCIONAL — comparação com solução exata
        // Para desativar, basta comentar a linha abaixo:
        processarSolucaoExata("solucao.txt", vetorSolucao, ordemMatriz);
        // =======================================================
    }

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
