#ifndef GAUSS_H
#define GAUSS_H

/* ============================================================
   ENUM DE STATUS DE RETORNO
   ============================================================ */
typedef enum {
    GAUSS_OK = 0,
    GAUSS_SINGULAR,
    GAUSS_INCONSISTENTE
} GaussStatus;

/* ============================================================
   FLAGS GLOBAIS (DIAGNÓSTICO)
   ============================================================ */

/**
 * @brief Reinicia flags internas (ex.: pivô ~ 0).
 */
void gauss_reset_flags(void);

/**
 * @brief Retorna 1 se algum pivô ≈ 0 foi encontrado durante a execução.
 */
int gaussFlagPivoQuaseZero(void);

/* ============================================================
   ETAPAS DO MÉTODO DE GAUSS (TOTAL, COM TOLERÂNCIA)
   ============================================================ */

/**
 * @brief Executa a fase de eliminação com pivotamento total.
 *
 * @param matrizEstendida Matriz [A|b] (n × (n+1)).
 * @param ordemMatriz     Ordem da matriz.
 * @param tolerancia      Valor mínimo considerado como zero.
 * @return GAUSS_OK ou GAUSS_SINGULAR.
 */
GaussStatus eliminacao_total(double** matrizEstendida, int ordemMatriz, double tolerancia);

/**
 * @brief Executa a fase de substituição regressiva após a eliminação.
 *
 * @param matrizEstendida Matriz [U|c] (n × (n+1)).
 * @param ordemMatriz     Ordem da matriz.
 * @param vetorSolucao    Vetor solução (saída).
 * @param tolerancia      Valor mínimo considerado como zero.
 * @return GAUSS_OK, GAUSS_SINGULAR ou GAUSS_INCONSISTENTE.
 */
GaussStatus substituicaoRegressiva_total(double** matrizEstendida, int ordemMatriz,
                                         double* vetorSolucao, double tolerancia);

/**
 * @brief Resolve Ax = b pelo método de Gauss com pivotamento total (com tolerância).
 *
 * @param matrizEstendida Matriz [A|b] (n × (n+1)).
 * @param ordemMatriz     Ordem da matriz.
 * @param vetorSolucao    Vetor solução (saída).
 * @param tolerancia      Valor mínimo considerado como zero.
 * @return GAUSS_OK, GAUSS_SINGULAR ou GAUSS_INCONSISTENTE.
 */
GaussStatus gauss(double** matrizEstendida, int ordemMatriz,
                  double* vetorSolucao, double tolerancia);

/* ============================================================
   UTILITÁRIO
   ============================================================ */

/**
 * @brief Imprime mensagem textual correspondente ao status retornado.
 *
 * @param status Código de retorno.
 */
void imprimirStatus(GaussStatus status);

#endif /* GAUSS_H */
