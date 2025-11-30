#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "utils.h"
#include "lu.h"

// ============================================
// FUNÇÕES AUXILIARES
// ============================================

static void mostrarFatoracaoLU(double** matrizA, int ordemMatriz, double tolerancia) {
    double** matrizL = alocarMatriz(ordemMatriz);
    double** matrizU = alocarMatriz(ordemMatriz);

    luDecomposicao(matrizA, ordemMatriz, matrizL, matrizU, tolerancia);

    imprimirMatriz("L:", matrizL, ordemMatriz);
    imprimirMatriz("U:", matrizU, ordemMatriz);

    double determinante = 1.0;
    for (int i = 0; i < ordemMatriz; i++)
        determinante *= matrizU[i][i];

    printf("det(A) (produto dos pivos de U) = %.6e\n\n", determinante);

    liberarMatriz(matrizL, ordemMatriz);
    liberarMatriz(matrizU, ordemMatriz);
}

static void verificarInversa(double** matrizA, double** matrizInversa,
                             int ordemMatriz, double** matrizErros) {
    double** matrizProduto = alocarMatriz(ordemMatriz);
    multiplicarMatrizes(matrizA, matrizInversa, matrizProduto, ordemMatriz);

    imprimirMatriz("A * A^{-1}:", matrizProduto, ordemMatriz);

    double erroMaximo = calcularErrosIdentidadeComMax(matrizProduto, ordemMatriz, matrizErros);
    imprimirMatriz("Erro elemento-a-elemento para I:", matrizErros, ordemMatriz);
    printf("Max |R - I| = %.6e\n", erroMaximo);

    liberarMatriz(matrizProduto, ordemMatriz);
}

// ============================================
// FUNÇÃO PRINCIPAL
// ============================================

int main(void) {
    int ordemMatriz = 3;
    double** matrizA       = criarMatrizA1();  // altere aqui para testar outra matriz
    double** matrizInversa = alocarMatriz(ordemMatriz);
    double** matrizErros   = alocarMatriz(ordemMatriz);
    double tolerancia      = 1e-12;  // limite para detectar pivôs quase nulos (baseado na precisão do double)

    double** matrizCopia = clonarMatriz(matrizA, ordemMatriz);

    // ============================================
    // MEDIR TEMPO DE EXECUÇÃO (clock_gettime)
    // ============================================
    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    (void) inversaPorLU(matrizA, ordemMatriz, tolerancia, matrizInversa);

    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempo_execucao =
        (double)(fim.tv_sec - inicio.tv_sec) +
        (double)(fim.tv_nsec - inicio.tv_nsec) / 1e9;

    // ============================================
    // RESULTADOS E ANÁLISE
    // ============================================
    if (luFlagPivoQuaseZero()) {
        puts("[AVISO] Pivo aproximado de zero detectado: matriz singular ou quase singular. "
             "Os resultados a seguir sao numericamente instaveis e potencialmente invalidos.");
    }

    mostrarFatoracaoLU(matrizCopia, ordemMatriz, tolerancia);
    imprimirMatriz("A^{-1} (calculada):", matrizInversa, ordemMatriz);
    verificarInversa(matrizCopia, matrizInversa, ordemMatriz, matrizErros);

    liberarMatriz(matrizA, ordemMatriz);
    liberarMatriz(matrizInversa, ordemMatriz);
    liberarMatriz(matrizErros, ordemMatriz);
    liberarMatriz(matrizCopia, ordemMatriz);

    printf("\nTempo de execucao total: %.6e segundos\n\n", tempo_execucao);

    return 0;
}
