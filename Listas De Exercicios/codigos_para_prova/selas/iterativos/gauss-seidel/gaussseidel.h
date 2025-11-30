#ifndef GAUSS_SEIDEL_H
#define GAUSS_SEIDEL_H

/**
 * @brief Códigos de retorno para o método de Gauss-Seidel.
 */
typedef enum {
    GS_OK = 0,            /**< Execução normal */
    GS_SINGULAR = 1,      /**< Pivô ~ 0 → sistema singular/indeterminado */
    GS_NAO_CONVERGIU = 2  /**< Método não convergiu em maxIter */
} GaussSeidelStatus;

/**
 * @brief Resolve Ax = b pelo método de Gauss-Seidel.
 *
 * @param matrizEstendida Matriz estendida [A|b] (n × (n+1)).
 * @param ordemMatriz Ordem da matriz A.
 * @param vetorSolucao Vetor solução (saída), tamanho n.
 * @param tolerancia Critério de parada relativo (norma do erro).
 * @param maxIter Número máximo de iterações.
 * @return GS_OK, GS_SINGULAR ou GS_NAO_CONVERGIU.
 */
GaussSeidelStatus gaussSeidel(double** matrizEstendida, int ordemMatriz,
                              double* vetorSolucao, double tolerancia, long long int maxIter);

/**
 * @brief Imprime em texto o status retornado pelo Gauss-Seidel.
 *
 * @param status Código de retorno.
 */
void imprimirStatusGaussSeidel(GaussSeidelStatus status);

#endif
