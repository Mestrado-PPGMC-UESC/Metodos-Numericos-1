/**
 * @file gauss.h
 * @brief Cabeçalhos e definições comuns para o método de Gauss
 *        (pivotamento parcial, por pesos, etc.)
 */

#ifndef GAUSS_H
#define GAUSS_H

#include <stdio.h>

/* ======================================================================
 * ENUM DE STATUS — usado em todos os módulos do Gauss
 * ====================================================================== */
typedef enum {
    GAUSS_OK = 0,            /**< Sistema resolvido normalmente            */
    GAUSS_SINGULAR,          /**< Sistema singular ou numericamente instável */
    GAUSS_INCONSISTENTE      /**< Sistema inconsistente (sem solução)     */
} GaussStatus;

/* ======================================================================
 * FUNÇÕES PÚBLICAS
 * ====================================================================== */

/**
 * @brief Reinicia as flags de diagnóstico globais.
 */
void gauss_reset_flags(void);

/**
 * @brief Retorna 1 se algum pivô ≈ 0 foi encontrado.
 */
int gaussFlagPivoQuaseZero(void);

/**
 * @brief Executa a eliminação de Gauss (pivotamento parcial, total ou por pesos),
 *        conforme implementado em cada módulo.
 *
 * @param matrizEstendida Matriz aumentada [A|b].
 * @param ordemMatriz     Ordem da matriz A.
 * @param tolerancia      Valor limite para considerar pivôs nulos.
 * @return Status intermediário (OK ou INCONSISTENTE).
 */
GaussStatus eliminacao_parcial(double** matrizEstendida, int ordemMatriz, double tolerancia);

/**
 * @brief Resolve o sistema triangular superior Ux = c.
 *
 * @param matrizEstendida Matriz [U|c].
 * @param ordemMatriz     Ordem da matriz.
 * @param vetorSolucao    Vetor solução x.
 * @param tolerancia      Tolerância numérica.
 * @return Status intermediário (OK ou INCONSISTENTE).
 */
GaussStatus substituicaoRegressiva(double** matrizEstendida, int ordemMatriz,
                                   double* vetorSolucao, double tolerancia);

/**
 * @brief Resolve Ax = b via eliminação de Gauss completa (wrapper).
 *
 * @param matrizEstendida Matriz aumentada [A|b].
 * @param ordemMatriz     Ordem da matriz A.
 * @param vetorSolucao    Vetor x a preencher.
 * @param tolerancia      Tolerância para pivôs pequenos.
 * @return GAUSS_OK, GAUSS_SINGULAR ou GAUSS_INCONSISTENTE.
 */
GaussStatus gauss(double** matrizEstendida, int ordemMatriz,
                  double* vetorSolucao, double tolerancia);

/**
 * @brief Imprime mensagem textual correspondente ao status retornado.
 */
void imprimirStatus(GaussStatus status);

#endif /* GAUSS_H */
