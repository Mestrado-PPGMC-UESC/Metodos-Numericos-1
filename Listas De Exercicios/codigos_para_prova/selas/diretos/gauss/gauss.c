/**
 * @file gauss.c
 * @brief Implementação do método de Eliminação de Gauss sem pivotamento,
 *        com detecção de pivôs pequenos e inconsistências numéricas.
 */

#include <stdio.h>
#include <math.h>
#include "gauss.h"

/* ======================================================================
 * FLAGS GLOBAIS DE DIAGNÓSTICO
 * ====================================================================== */

static int g_gauss_pivo_quase_zero = 0; /**< Flag global: indica presença de pivô ≈ 0. */

/**
 * @brief Reinicia as flags internas do método de Gauss.
 */
void gauss_reset_flags(void) {
    g_gauss_pivo_quase_zero = 0;
}

/**
 * @brief Retorna 1 se algum pivô ≈ 0 foi encontrado durante a execução.
 */
int gaussFlagPivoQuaseZero(void) {
    return g_gauss_pivo_quase_zero;
}

/* ======================================================================
 * ELIMINAÇÃO DE GAUSS (SEM PIVOTAMENTO)
 * ====================================================================== */

/**
 * @brief Executa a etapa de eliminação de Gauss com tolerância numérica.
 *
 * Se um pivô for considerado pequeno (|pivô| < tolerância), o método
 * marca a flag e segue o cálculo, sem abortar o processo. No final,
 * verifica inconsistências do tipo 0...0 | b≠0.
 *
 * @param matrizEstendida Matriz aumentada [A|b] (n × (n+1)), modificada in-place.
 * @param ordemMatriz Ordem n da matriz quadrada A.
 * @param tolerancia Valor mínimo para considerar um pivô ≈ 0 (ex.: 1e-12).
 * @return
 * - `GAUSS_OK` se a eliminação foi concluída sem incidentes;
 * - `GAUSS_SINGULAR` se houve pivô ≈ 0 (instabilidade numérica);
 * - `GAUSS_INCONSISTENTE` se uma linha 0...0 | b≠0 for detectada.
 */
GaussStatus eliminacao(double** matrizEstendida, int ordemMatriz, double tolerancia) {
    int flag_singular = 0;

    for (int indicePivo = 0; indicePivo < ordemMatriz - 1; indicePivo++) {
        double pivo = matrizEstendida[indicePivo][indicePivo];

        if (fabs(pivo) < tolerancia) {
            g_gauss_pivo_quase_zero = 1;
            flag_singular = 1;
            continue;  // segue para a próxima coluna
        }

        for (int linhaAtual = indicePivo + 1; linhaAtual < ordemMatriz; linhaAtual++) {
            double multiplicador = matrizEstendida[linhaAtual][indicePivo] / pivo;
            if (multiplicador == 0.0) continue;

            for (int colunaAtual = indicePivo; colunaAtual <= ordemMatriz; colunaAtual++) {
                matrizEstendida[linhaAtual][colunaAtual] -=
                    multiplicador * matrizEstendida[indicePivo][colunaAtual];
            }
        }
    }

    // Checagem final: linha nula com termo independente não nulo
    for (int linhaAtual = 0; linhaAtual < ordemMatriz; linhaAtual++) {
        int linhaZero = 1;
        for (int colunaAtual = 0; colunaAtual < ordemMatriz; colunaAtual++) {
            if (fabs(matrizEstendida[linhaAtual][colunaAtual]) >= tolerancia) {
                linhaZero = 0;
                break;
            }
        }
        if (linhaZero && fabs(matrizEstendida[linhaAtual][ordemMatriz]) >= tolerancia)
            return GAUSS_INCONSISTENTE;
    }

    return flag_singular ? GAUSS_SINGULAR : GAUSS_OK;
}

/* ======================================================================
 * SUBSTITUIÇÃO REGRESSIVA
 * ====================================================================== */

/**
 * @brief Resolve o sistema triangular superior Ux = c.
 *
 * Caso encontre um pivô ≈ 0:
 * - Se o termo independente também for ≈ 0 → variável livre (x=0);
 * - Se o termo independente for significativo → sistema inconsistente.
 *
 * @param matrizEstendida Matriz aumentada [U|c] (triangular superior).
 * @param ordemMatriz Ordem n da matriz.
 * @param vetorSolucao Vetor destino onde será armazenada a solução x (tamanho n).
 * @param tolerancia Valor mínimo para considerar um pivô ≈ 0.
 * @return
 * - `GAUSS_OK` se o sistema for resolvido;
 * - `GAUSS_SINGULAR` se houve pivô ≈ 0;
 * - `GAUSS_INCONSISTENTE` se 0*x = b≠0 for detectado.
 */
GaussStatus substituicaoRegressiva(double** matrizEstendida, int ordemMatriz,
                                   double* vetorSolucao, double tolerancia) {
    int flag_singular = 0;

    for (int linha = ordemMatriz - 1; linha >= 0; linha--) {
        double somaAcima = 0.0;
        for (int coluna = linha + 1; coluna < ordemMatriz; coluna++) {
            somaAcima += matrizEstendida[linha][coluna] * vetorSolucao[coluna];
        }

        double diag = matrizEstendida[linha][linha];
        double termoIndepAjustado = matrizEstendida[linha][ordemMatriz] - somaAcima;

        if (fabs(diag) < tolerancia) {
            if (fabs(termoIndepAjustado) >= tolerancia)
                return GAUSS_INCONSISTENTE;  // 0*x = b≠0

            g_gauss_pivo_quase_zero = 1;
            flag_singular = 1;
            vetorSolucao[linha] = termoIndepAjustado / diag;
            continue;
        }

        vetorSolucao[linha] = termoIndepAjustado / diag;
    }

    return flag_singular ? GAUSS_SINGULAR : GAUSS_OK;
}

/* ======================================================================
 * FUNÇÃO PRINCIPAL DO MÉTODO DE GAUSS
 * ====================================================================== */

/**
 * @brief Resolve o sistema linear Ax = b via eliminação de Gauss sem pivotamento.
 *
 * Combina eliminação e substituição regressiva, propagando o status apropriado.
 * Retorna `GAUSS_SINGULAR` caso qualquer pivô ≈ 0 tenha sido detectado.
 *
 * @param matrizEstendida Matriz aumentada [A|b] (n × (n+1)).
 * @param ordemMatriz Ordem n da matriz.
 * @param vetorSolucao Vetor destino onde será armazenada a solução x (tamanho n).
 * @param tolerancia Valor mínimo para considerar pivô ≈ 0.
 * @return
 * - `GAUSS_OK` se a resolução foi estável;
 * - `GAUSS_SINGULAR` se houve pivôs pequenos (instabilidade);
 * - `GAUSS_INCONSISTENTE` se o sistema não possuir solução.
 */
GaussStatus gauss(double** matrizEstendida, int ordemMatriz,
                  double* vetorSolucao, double tolerancia) {
    gauss_reset_flags();

    GaussStatus status = eliminacao(matrizEstendida, ordemMatriz, tolerancia);
    if (status == GAUSS_INCONSISTENTE)
        return GAUSS_INCONSISTENTE;

    GaussStatus statusReg = substituicaoRegressiva(matrizEstendida, ordemMatriz, vetorSolucao, tolerancia);
    if (statusReg == GAUSS_INCONSISTENTE)
        return GAUSS_INCONSISTENTE;

    return (status == GAUSS_SINGULAR || statusReg == GAUSS_SINGULAR)
           ? GAUSS_SINGULAR : GAUSS_OK;
}

/* ======================================================================
 * FUNÇÃO AUXILIAR DE IMPRESSÃO DE STATUS
 * ====================================================================== */

/**
 * @brief Exibe mensagem textual correspondente ao status retornado.
 *
 * @param status Código do tipo GaussStatus retornado pelas funções.
 */
void imprimirStatus(GaussStatus status) {
    if (status == GAUSS_OK)
        puts("OK");
    else if (status == GAUSS_SINGULAR)
        puts("Sistema singular ou numericamente instável (pivo ~ 0).");
    else if (status == GAUSS_INCONSISTENTE)
        puts("Sistema inconsistente (linha zero em A com b != 0).");
}
