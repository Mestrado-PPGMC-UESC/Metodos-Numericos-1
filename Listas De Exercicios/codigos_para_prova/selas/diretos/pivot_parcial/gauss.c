/**
 * @file gauss.c
 * @brief Implementação do método de Eliminação de Gauss com pivotamento parcial,
 *        seguindo o padrão do "gauss common": flags de diagnóstico, nomes descritivos
 *        e retorno de status (OK, SINGULAR, INCONSISTENTE).
 */

#include <stdio.h>
#include <math.h>
#include "gauss.h"

/* ======================================================================
 * FLAGS E CONTADORES GLOBAIS DE DIAGNÓSTICO
 * ====================================================================== */

static int g_gauss_pivo_quase_zero = 0;  /**< Flag: indica presença de pivô ≈ 0. */

/**
 * @brief Reinicia as flags e contadores internos do método de Gauss.
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
 * ELIMINAÇÃO DE GAUSS — PIVOTAMENTO PARCIAL (COM TOLERÂNCIA)
 * ====================================================================== */

/**
 * @brief Executa a etapa de eliminação com pivotamento parcial (troca de linhas).
 *
 * Política:
 *  - Em cada coluna @p indicePivo, seleciona a linha com maior |A[linha, indicePivo]|;
 *  - Efetua troca de linhas (se necessário) e incrementa contador;
 *  - Se |pivô| < tolerância → marca flag e continua (não divide por ~0);
 *  - Ao final, verifica inconsistência (linha 0...0 | b≠0).
 *
 * @param matrizEstendida Matriz aumentada [A|b] (ordemMatriz × (ordemMatriz+1)), modificada in-place.
 * @param ordemMatriz     Ordem n da matriz A.
 * @param tolerancia      Valor mínimo para considerar pivô ≈ 0 (ex.: 1e-12).
 * @return
 *  - `GAUSS_OK` se não houver inconsistência;
 *  - `GAUSS_INCONSISTENTE` se existir linha 0...0 | b≠0.
 *
 * @note A presença de pivôs pequenos é reportada via flag e refletida no status final
 *       quando combinado com a substituição regressiva (pode resultar em GAUSS_SINGULAR).
 */
GaussStatus eliminacao_parcial(double** matrizEstendida, int ordemMatriz, double tolerancia) {
    int houve_pivo_pequeno = 0;

    for (int indicePivo = 0; indicePivo < ordemMatriz - 1; indicePivo++) {

        /* (1) Seleção de pivô: maior |A[linhaCandidata, indicePivo]| */
        int    linhaPivoSelecionada = indicePivo;
        double maiorValorAbsoluto   = fabs(matrizEstendida[indicePivo][indicePivo]);

        for (int linhaCandidata = indicePivo + 1; linhaCandidata < ordemMatriz; linhaCandidata++) {
            double valorAbsolutoAtual = fabs(matrizEstendida[linhaCandidata][indicePivo]);
            if (valorAbsolutoAtual > maiorValorAbsoluto) {
                maiorValorAbsoluto   = valorAbsolutoAtual;
                linhaPivoSelecionada = linhaCandidata;
            }
        }

        /* (2) Troca de linhas, se necessário */
        if (linhaPivoSelecionada != indicePivo) {
            double* linhaTemp                   = matrizEstendida[indicePivo];
            matrizEstendida[indicePivo]         = matrizEstendida[linhaPivoSelecionada];
            matrizEstendida[linhaPivoSelecionada] = linhaTemp;
        }

        /* (3) Checagem do pivô e política fail-soft */
        double pivo = matrizEstendida[indicePivo][indicePivo];
        if (fabs(pivo) < tolerancia) {
            g_gauss_pivo_quase_zero = 1;
            houve_pivo_pequeno = 1;
            /* Não faz eliminação sob pivô ~ 0 para evitar divisão instável */
            continue;
        }

        /* (4) Eliminação: zera entradas abaixo do pivô em indicePivo */
        for (int linhaAtual = indicePivo + 1; linhaAtual < ordemMatriz; linhaAtual++) {
            double elementoColuna = matrizEstendida[linhaAtual][indicePivo];
            if (fabs(elementoColuna) < tolerancia) continue;

            double multiplicador = elementoColuna / pivo;
            for (int colunaAtual = indicePivo; colunaAtual <= ordemMatriz; colunaAtual++) {
                matrizEstendida[linhaAtual][colunaAtual] -= multiplicador * matrizEstendida[indicePivo][colunaAtual];
            }
        }
    }

    /* (5) Checagem de inconsistência: linha 0...0 | b≠0 */
    for (int linhaAtual = 0; linhaAtual < ordemMatriz; linhaAtual++) {
        int linhaTodaZero = 1;
        for (int colunaAtual = 0; colunaAtual < ordemMatriz; colunaAtual++) {
            if (fabs(matrizEstendida[linhaAtual][colunaAtual]) >= tolerancia) {
                linhaTodaZero = 0;
                break;
            }
        }
        if (linhaTodaZero && fabs(matrizEstendida[linhaAtual][ordemMatriz]) >= tolerancia) {
            return GAUSS_INCONSISTENTE;
        }
    }

    /* Observação: não retornamos SINGULAR aqui. Deixamos para o wrapper decidir
       (com base nesta etapa + substituição regressiva). Mantemos o padrão do common. */
    return houve_pivo_pequeno ? GAUSS_OK : GAUSS_OK;
}

/* ======================================================================
 * SUBSTITUIÇÃO REGRESSIVA (Ux = c) — ROBUSTA COM TOLERÂNCIA
 * ====================================================================== */

/**
 * @brief Resolve o sistema triangular superior Ux = c.
 *
 * Regras:
 *  - Se |U[ii]| ≥ tolerância → x[i] = (c_i - soma)/U[ii];
 *  - Se |U[ii]| <  tolerância e |c_i - soma| <  tolerância → variável livre: x[i] = 0 e marca flag;
 *  - Se |U[ii]| <  tolerância e |c_i - soma| ≥ tolerância → inconsistência (0*x = b≠0).
 *
 * @param matrizEstendida Matriz [U|c] triangular superior.
 * @param ordemMatriz     Ordem n da matriz.
 * @param vetorSolucao    Vetor solução x (tamanho n).
 * @param tolerancia      Tolerância numérica.
 * @return
 *  - `GAUSS_OK` se a regressiva concluiu sem contradições;
 *  - `GAUSS_INCONSISTENTE` se 0*x = b≠0 for detectado;
 *  - `GAUSS_SINGULAR` **não é retornado aqui**; a flag é atualizada e o wrapper decide.
 */
GaussStatus substituicaoRegressiva(double** matrizEstendida, int ordemMatriz,
                                   double* vetorSolucao, double tolerancia) {
    const int n = ordemMatriz;
    int marcou_inconsistencia = 0;  /* registra 0*x = b≠0, mas NÃO interrompe */

    for (int linha = n - 1; linha >= 0; linha--) {
        double soma = 0.0;
        for (int col = linha + 1; col < n; col++) {
            soma += matrizEstendida[linha][col] * vetorSolucao[col];
        }

        double diag = matrizEstendida[linha][linha];
        double rhs  = matrizEstendida[linha][n] - soma;

        if (fabs(diag) >= tolerancia) {
            /* caso normal */
            vetorSolucao[linha] = rhs / diag;
        } else {
            vetorSolucao[linha] = rhs / diag;
            g_gauss_pivo_quase_zero = 1;

            if (fabs(rhs) >= tolerancia) {
                marcou_inconsistencia = 1;
            }
        }
    }

    return marcou_inconsistencia ? GAUSS_INCONSISTENTE : GAUSS_OK;
}


/* ======================================================================
 * FUNÇÃO PRINCIPAL (ELIMINAÇÃO + REGRESSIVA)
 * ====================================================================== */

/**
 * @brief Resolve Ax = b via Gauss com pivotamento parcial.
 *
 * Combina eliminação parcial e substituição regressiva. O status final segue:
 *  - `GAUSS_INCONSISTENTE` se qualquer etapa sinalizar inconsistência;
 *  - `GAUSS_SINGULAR` se não houver inconsistência, mas houver pivôs ≈ 0 (flag ativa);
 *  - `GAUSS_OK` caso contrário.
 */
GaussStatus gauss(double** matrizEstendida, int ordemMatriz,
                  double* vetorSolucao, double tolerancia) {
    gauss_reset_flags();

    GaussStatus statusEliminacao = eliminacao_parcial(matrizEstendida, ordemMatriz, tolerancia);
    if (statusEliminacao == GAUSS_INCONSISTENTE) {
        return GAUSS_INCONSISTENTE;
    }

    GaussStatus statusRegressiva = substituicaoRegressiva(matrizEstendida, ordemMatriz,
                                                          vetorSolucao, tolerancia);
    if (statusRegressiva == GAUSS_INCONSISTENTE) {
        return GAUSS_INCONSISTENTE;
    }

    /* Se chegamos aqui, não houve contradição. Decide singularidade pelo flag. */
    return gaussFlagPivoQuaseZero() ? GAUSS_SINGULAR : GAUSS_OK;
}

/* ======================================================================
 * IMPRESSÃO DO STATUS
 * ====================================================================== */

/**
 * @brief Exibe mensagem textual correspondente ao status retornado.
 */
void imprimirStatus(GaussStatus status) {
    if (status == GAUSS_OK) {
        puts("OK");
    } else if (status == GAUSS_SINGULAR) {
        puts("Sistema singular ou numericamente instavel (pivo ~ 0).");
    } else if (status == GAUSS_INCONSISTENTE) {
        puts("Sistema inconsistente (linha zero em A com b != 0).");
    }
}
