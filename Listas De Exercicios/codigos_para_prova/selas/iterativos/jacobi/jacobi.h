#ifndef JACOBI_H
#define JACOBI_H

/**
 * @brief Códigos de retorno do método de Jacobi.
 */
typedef enum {
    JACOBI_OK = 0,           /**< Convergiu dentro de maxIter com a tolerância dada. */
    JACOBI_SINGULAR = 1,     /**< Algum a_ii ~ 0 (divisão por zero/instável) ou falha de alocação. */
    JACOBI_NAO_CONVERGIU = 2 /**< Atingiu maxIter sem satisfazer o critério de parada. */
} JacobiStatus;

/**
 * @brief Resolve Ax = b pelo método iterativo de Jacobi usando a matriz estendida [A|b].
 *
 * Fórmula de atualização (para i = 0..n-1):
 *   x_i^{k+1} = ( b_i - sum_{j != i} a_ij * x_j^{k} ) / a_ii
 *
 * Critério de parada:
 *   ||x^{k+1} - x^{k}||_inf < tolerancia  (ou até atingir maxIter)
 *
 * @param matrizEstendida Matriz [A|b] (n × (n+1)).
 * @param ordemMatriz Ordem da matriz A (n).
 * @param vetorSolucao Vetor solução (saída), tamanho n.
 * @param tolerancia Tolerância para o critério de parada (ex.: 1e-8).
 * @param maxIter Número máximo de iterações (ex.: 10000).
 * @return JACOBI_OK, JACOBI_SINGULAR ou JACOBI_NAO_CONVERGIU.
 */
JacobiStatus jacobi(double** matrizEstendida, int ordemMatriz,
                    double* vetorSolucao, double tolerancia, long long int maxIter);

/**
 * @brief Imprime em texto o status retornado pelo método de Jacobi.
 *
 * @param status Código de retorno.
 */
void imprimirStatusJacobi(JacobiStatus status);

#endif
