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
 * @brief Programa principal para resolução de sistemas lineares via Método de Gauss com pivotamento parcial.
 *
 * Lê de um arquivo texto uma matriz aumentada [A|b],
 * resolve o sistema com pivotamento parcial, mede o tempo de execução
 * e, opcionalmente, compara com uma solução exata lida de outro arquivo.
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

    // Método de Gauss com pivotamento parcial (fail-soft)
    GaussStatus status = gauss(matrizEstendida, ordemMatriz, vetorSolucao, tolerancia);

    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempoSegundos = diffTempo(&inicio, &fim);
    double tempoNanoSeg  = tempoSegundos * 1e9;

    // ============================================
    // ETAPA 3: Exibir resultados
    // ============================================
    imprimirStatus(status);

    if (status == GAUSS_INCONSISTENTE) {
        puts("[ERRO] Sistema inconsistente: não há solução (linha nula em A com b != 0).");
    } else {
        if (status == GAUSS_SINGULAR || gaussFlagPivoQuaseZero()) {
            puts("[AVISO] Pivo aproximado de zero detectado: sistema singular ou numericamente instavel.");
        }

        // Exibir resultados mesmo que instável
        imprimirSolucao(vetorSolucao, ordemMatriz);

        // =======================================================
        // BLOCO OPCIONAL — comparação com solução exata
        // (segue o mesmo padrão do gauss "comum")
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

    // Código de saída: 0 = OK, 1 = singular/instável, 2 = inconsistente
    if (status == GAUSS_OK)       return 0;
    if (status == GAUSS_SINGULAR) return 1;
    return 2;
}
