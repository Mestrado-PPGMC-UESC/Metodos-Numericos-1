#ifndef GAUSS_H
#define GAUSS_H

/**
 * @file gauss.h
 * @brief Declarações das rotinas do método de Eliminação de Gauss com pivotamento parcial,
 *        seguindo o mesmo padrão do módulo "gauss common": não aborta por pivô pequeno;
 *        apenas sinaliza via flags; retorna INCONSISTENTE apenas em 0...0 | b≠0.
 *
 * Convenções:
 *  - Matriz estendida [A|b] com n linhas e (n+1) colunas.
 *  - Índices 0-based.
 *  - Tolerância passada por parâmetro controla testes de quase singularidade.
 */

/* ======================================================================
 * ENUMERAÇÃO DE STATUS
 * ====================================================================== */

/**
 * @enum GaussStatus
 * @brief Códigos de retorno das rotinas do método de Gauss.
 */
typedef enum {
    GAUSS_OK = 0,           /**< Execução normal (sem contradições detectadas). */
    GAUSS_SINGULAR = 1,     /**< Reservado (não usado para abortar; ver flag interna). */
    GAUSS_INCONSISTENTE = 2 /**< Sistema inconsistente (linha nula em A com b ≠ 0). */
} GaussStatus;

/* ======================================================================
 * FUNÇÕES PRINCIPAIS
 * ====================================================================== */

/**
 * @brief Executa a etapa de eliminação de Gauss com pivotamento parcial (fail-soft).
 *
 * Transforma [A|b] em forma triangular superior [U|c], escolhendo em cada coluna k
 * a linha i ≥ k que maximiza |A[i,k]| e trocando k ↔ i. Pivôs ≈ 0 não interrompem
 * o processo — apenas disparam flag interna.
 *
 * Após a eliminação, verifica inconsistência (linhas 0...0 | b ≠ 0).
 *
 * @param matrizEstendida Matriz aumentada [A|b] (dimensão n × (n+1)), modificada in-place.
 * @param ordemMatriz Ordem n da matriz quadrada A.
 * @param tolerancia Valor mínimo para considerar pivô ≈ 0 (ex.: 1e-12).
 * @return
 * - `GAUSS_OK` se o processo foi bem-sucedido.
 * - `GAUSS_INCONSISTENTE` se houver linha 0...0 | b ≠ 0.
 */
GaussStatus eliminacao_parcial(double** matrizEstendida, int ordemMatriz, double tolerancia);

/**
 * @brief Realiza a substituição regressiva (resolve Ux = c) de forma robusta.
 *
 * Se |U[i,i]| ≲ tolerância e |c_i - soma| ≲ tolerância, define x[i] = 0 e marca flag.
 * Se |U[i,i]| ≲ tolerância e |c_i - soma| > tolerância, retorna INCONSISTENTE.
 *
 * @param matrizEstendida Matriz [U|c] (triangular superior).
 * @param ordemMatriz Ordem n da matriz.
 * @param vetorSolucao Vetor destino onde será armazenada a solução x (tamanho n).
 * @param tolerancia Valor mínimo para considerar pivô ≈ 0.
 * @return
 * - `GAUSS_OK` se o sistema foi resolvido.
 * - `GAUSS_INCONSISTENTE` se houver equação impossível (0*x = b≠0).
 */
GaussStatus substituicaoRegressiva(double** matrizEstendida, int ordemMatriz,
                                   double* vetorSolucao, double tolerancia);

/**
 * @brief Resolve o sistema linear Ax = b pelo método de Gauss com pivotamento parcial.
 *
 * Combina a eliminação e a substituição regressiva, com detecção de inconsistência.
 * Pivôs pequenos não causam interrupção — apenas ativam a flag global.
 *
 * @param matrizEstendida Matriz aumentada [A|b] (n × (n+1)).
 * @param ordemMatriz Ordem n da matriz A.
 * @param vetorSolucao Vetor solução (tamanho n).
 * @param tolerancia Valor mínimo para considerar pivô ≈ 0.
 * @return
 * - `GAUSS_OK` se o sistema foi resolvido.
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
 * @brief Reseta as flags internas do módulo de Gauss.
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
