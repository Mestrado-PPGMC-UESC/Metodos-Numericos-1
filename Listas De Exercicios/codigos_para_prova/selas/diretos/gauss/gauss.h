#ifndef GAUSS_H
#define GAUSS_H

/**
 * @file gauss.h
 * @brief Declarações das rotinas do método de Eliminação de Gauss (sem pivotamento),
 *        incluindo detecção de singularidade e inconsistência com base em tolerância.
 *
 * Este módulo implementa a eliminação de Gauss com tratamento numérico estável,
 * sinalizando situações de pivôs pequenos ou linhas inconsistentes.
 * Não realiza pivotamento, sendo útil para fins de análise de estabilidade e propagação de erros.
 */

/* ======================================================================
 * ENUMERAÇÃO DE STATUS
 * ====================================================================== */

/**
 * @enum GaussStatus
 * @brief Códigos de retorno das rotinas do método de Gauss.
 */
typedef enum {
    GAUSS_OK = 0,           /**< Execução normal (sem falhas numéricas detectadas). */
    GAUSS_SINGULAR = 1,     /**< Sistema singular ou indeterminado (pivô ≈ 0). */
    GAUSS_INCONSISTENTE = 2 /**< Sistema inconsistente (linha nula em A com b ≠ 0). */
} GaussStatus;

/* ======================================================================
 * FUNÇÕES PRINCIPAIS
 * ====================================================================== */

/**
 * @brief Executa a etapa de eliminação de Gauss (sem pivotamento), com tolerância integrada.
 *
 * Transforma a matriz aumentada [A|b] em forma triangular superior, aplicando guardas numéricas.
 * Não aborta quando encontra pivôs pequenos — apenas sinaliza via flag global.
 * Também verifica inconsistência após a eliminação (linhas nulas com b ≠ 0).
 *
 * @param matrizEstendida Matriz aumentada [A|b] (dimensão n × (n+1)), modificada in-place.
 * @param ordemMatriz Ordem n da matriz quadrada A.
 * @param tolerancia Valor mínimo para considerar um pivô ≈ 0 (ex.: 1e-12).
 * @return
 * - `GAUSS_OK` se o processo foi bem-sucedido.
 * - `GAUSS_INCONSISTENTE` se alguma linha 0...0 | b ≠ 0 for detectada.
 */
GaussStatus eliminacao(double** matrizEstendida, int ordemMatriz, double tolerancia);

/**
 * @brief Realiza a substituição regressiva (resolve Ux = c) de forma robusta.
 *
 * Processa o sistema triangular superior resultante da eliminação.
 * Se um pivô for ≈ 0 e o termo independente também ≈ 0, define x[i] = 0 e marca a flag global.
 * Se o pivô ≈ 0 e o termo independente for significativo, o sistema é inconsistente.
 *
 * @param matrizEstendida Matriz aumentada [U|c] (triangular superior).
 * @param ordemMatriz Ordem n da matriz.
 * @param vetorSolucao Vetor destino onde será armazenada a solução x (tamanho n).
 * @param tolerancia Valor mínimo para considerar um pivô ≈ 0.
 * @return
 * - `GAUSS_OK` se o sistema foi resolvido.
 * - `GAUSS_INCONSISTENTE` se alguma equação 0*x = b≠0 for detectada.
 */
GaussStatus substituicaoRegressiva(double** matrizEstendida, int ordemMatriz,
                                   double* vetorSolucao, double tolerancia);

/**
 * @brief Resolve o sistema linear Ax = b pelo método de Gauss sem pivotamento.
 *
 * Combina a eliminação e a substituição regressiva com tratamento de tolerância.
 * Nunca aborta o processo por pivô pequeno; apenas emite flags e retorna o status adequado.
 *
 * @param matrizEstendida Matriz aumentada [A|b] (n × (n+1)).
 * @param ordemMatriz Ordem n da matriz A.
 * @param vetorSolucao Vetor solução (tamanho n).
 * @param tolerancia Valor mínimo para considerar pivô ≈ 0.
 * @return
 * - `GAUSS_OK` se o sistema foi resolvido (possivelmente com avisos).
 * - `GAUSS_INCONSISTENTE` se o sistema não possuir solução.
 */
GaussStatus gauss(double** matrizEstendida, int ordemMatriz,
                  double* vetorSolucao, double tolerancia);

/* ======================================================================
 * FUNÇÕES DE DIAGNÓSTICO E FLAG
 * ====================================================================== */

/**
 * @brief Exibe no console uma mensagem textual correspondente ao status de Gauss.
 *
 * @param status Código de status retornado por alguma função do método de Gauss.
 */
void imprimirStatus(GaussStatus status);

/**
 * @brief Reseta as flags globais internas do módulo de Gauss.
 *
 * Deve ser chamada antes de iniciar um novo processo de resolução.
 */
void gauss_reset_flags(void);

/**
 * @brief Retorna 1 se algum pivô ≈ 0 foi encontrado durante a execução.
 *
 * Pode ser usada após chamar `gauss()` para verificar se houve instabilidade numérica.
 *
 * @return
 * - `1` se algum pivô foi considerado pequeno;
 * - `0` caso contrário.
 */
int gaussFlagPivoQuaseZero(void);

#endif /* GAUSS_H */
