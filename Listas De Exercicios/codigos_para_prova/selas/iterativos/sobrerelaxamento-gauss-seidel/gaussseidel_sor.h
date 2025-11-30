#ifndef GAUSS_SEIDEL_SOR_H
#define GAUSS_SEIDEL_SOR_H

/**
 * @brief Códigos de retorno para o método Gauss-Seidel com Sobre-Relaxamento (SOR).
 */
typedef enum {
    SOR_OK = 0,             /**< Execução normal */
    SOR_SINGULAR = 1,       /**< Pivô ~ 0 → sistema singular/indeterminado */
    SOR_NAO_CONVERGIU = 2,  /**< Método não convergiu em maxIter */
    SOR_PARAM_INVALIDO = 3  /**< Parâmetro omega fora do intervalo (0,2) */
} GaussSeidelSORStatus;

/**
 * @brief Resolve Ax = b pelo método Gauss-Seidel com Sobre-Relaxamento (SOR).
 *
 * Atualização por componente:
 *   x_i^{k+1} = (1 - omega) * x_i^{k} + omega * x_i^{GS}
 * onde
 *   x_i^{GS} = ( b_i - sum_{j<i} a_ij x_j^{k+1} - sum_{j>i} a_ij x_j^{k} ) / a_ii
 *
 * Critério de parada (relativo):
 *   | ||x^{k+1}||_inf - ||x^{k}||_inf | / max( ||x^{k+1}||_inf, ||x^{k}||_inf, eps ) < tolerancia
 *
 * @param matrizEstendida Matriz estendida [A|b] (n × (n+1)).
 * @param ordemMatriz Ordem da matriz A (n).
 * @param vetorSolucao Vetor solução (entrada/saída), tamanho n.
 * @param omega Fator de relaxamento, 0 < omega < 2 (omega=1 → Gauss-Seidel clássico).
 * @param tolerancia Tolerância para critério de parada relativo.
 * @param maxIter Número máximo de iterações.
 * @return SOR_OK, SOR_SINGULAR, SOR_NAO_CONVERGIU ou SOR_PARAM_INVALIDO.
 */
GaussSeidelSORStatus gaussSeidelSOR(double** matrizEstendida, int ordemMatriz,
                                    double* vetorSolucao, double omega,
                                    double tolerancia, long long int maxIter);

/**
 * @brief Imprime em texto o status retornado pelo SOR.
 *
 * @param status Código de retorno.
 */
void imprimirStatusGaussSeidelSOR(GaussSeidelSORStatus status);

#endif
