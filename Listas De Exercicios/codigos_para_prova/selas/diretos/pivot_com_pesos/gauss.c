/**
 * @file gauss.c
 * @brief Eliminação de Gauss com pivotamento por pesos (scaled partial) — FAIL-SOFT TOTAL.
 *
 * Política:
 * - Nunca interrompe por pivô pequeno nem por inconsistência durante o cálculo;
 * - Usa tolerância apenas para sinalizar flags internas;
 * - O status final (OK/SINGULAR/INCONSISTENTE) é decidido apenas no retorno de gauss().
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gauss.h"

/* ======================================================================
 * FLAGS GLOBAIS
 * ====================================================================== */

static int g_gauss_pivo_quase_zero = 0;   /* houve pivô ~ 0 em qualquer etapa */
static int g_gauss_inconsistente    = 0;   /* detectou 0...0 | b != 0 em alguma checagem */

/* Reinicia flags */
void gauss_reset_flags(void) {
    g_gauss_pivo_quase_zero = 0;
    g_gauss_inconsistente   = 0;
}

/* Exposição da flag de pivô ~0 (mantém padrão já usado) */
int gaussFlagPivoQuaseZero(void) {
    return g_gauss_pivo_quase_zero;
}

/* ======================================================================
 * ELIMINAÇÃO — pivotamento escalonado (com pesos) — FAIL-SOFT
 * ====================================================================== */

GaussStatus eliminacao_parcial(double** matrizEstendida, int ordemMatriz, double tolerancia) {
    const int n = ordemMatriz;

    /* (0) pesos s[i] = max_j |A[i,j]| */
    double* pesosLinha = (double*) malloc((size_t)n * sizeof(double));
    if (pesosLinha) {
        for (int linha = 0; linha < n; linha++) {
            double s = 0.0;
            for (int col = 0; col < n; col++) {
                double v = fabs(matrizEstendida[linha][col]);
                if (v > s) s = v;
            }
            pesosLinha[linha] = (s > 0.0 ? s : 1.0); /* evita div/0 no ratio */
        }
    } else {
        /* sem memória → segue sem escalonamento efetivo, mas não para */
        g_gauss_pivo_quase_zero = 1;
    }

    for (int indicePivo = 0; indicePivo < n - 1; indicePivo++) {

        /* (1) Seleção escalonada do pivô: maximiza |A[i,k]|/s[i] */
        int    linhaPivoSelecionada = indicePivo;
        double melhorRazao          = -1.0;

        for (int linhaCandidata = indicePivo; linhaCandidata < n; linhaCandidata++) {
            double numerador   = fabs(matrizEstendida[linhaCandidata][indicePivo]);
            double denominador = pesosLinha ? pesosLinha[linhaCandidata] : 1.0;
            double razao       = (denominador > 0.0) ? (numerador / denominador) : numerador;
            if (razao > melhorRazao) {
                melhorRazao          = razao;
                linhaPivoSelecionada = linhaCandidata;
            }
        }

        /* (2) Troca linhas/pesos, se necessário */
        if (linhaPivoSelecionada != indicePivo) {
            double* tmpL                         = matrizEstendida[indicePivo];
            matrizEstendida[indicePivo]          = matrizEstendida[linhaPivoSelecionada];
            matrizEstendida[linhaPivoSelecionada]= tmpL;

            if (pesosLinha) {
                double tmpS                      = pesosLinha[indicePivo];
                pesosLinha[indicePivo]           = pesosLinha[linhaPivoSelecionada];
                pesosLinha[linhaPivoSelecionada] = tmpS;
            }
        }

        /* (3) Pivô; se pequeno, marca flag e segue (não divide por ~0) */
        double pivo = matrizEstendida[indicePivo][indicePivo];
        if (fabs(pivo) < tolerancia) {
            g_gauss_pivo_quase_zero = 1;
            continue; /* não elimina nessa coluna; segue adiante */
        }

        /* (4) Eliminação abaixo do pivô */
        for (int linhaAtual = indicePivo + 1; linhaAtual < n; linhaAtual++) {
            double a_lk = matrizEstendida[linhaAtual][indicePivo];
            if (fabs(a_lk) < tolerancia) continue;

            double m = a_lk / pivo;
            for (int colunaAtual = indicePivo; colunaAtual <= n; colunaAtual++) {
                matrizEstendida[linhaAtual][colunaAtual] -=
                    m * matrizEstendida[indicePivo][colunaAtual];
            }
        }
    }

    /* (5) Checagem de inconsistência (não retorna! apenas marca flag) */
    for (int linha = 0; linha < n; linha++) {
        int linhaZero = 1;
        for (int col = 0; col < n; col++) {
            if (fabs(matrizEstendida[linha][col]) >= tolerancia) {
                linhaZero = 0; break;
            }
        }
        if (linhaZero && fabs(matrizEstendida[linha][n]) >= tolerancia) {
            g_gauss_inconsistente = 1;
            break; /* marca e segue; não interrompe */
        }
    }

    if (pesosLinha) free(pesosLinha);
    return GAUSS_OK; /* sempre OK aqui — decisão final fica para gauss() */
}

/* ======================================================================
 * SUBSTITUIÇÃO REGRESSIVA (Ux = c) — FAIL-SOFT
 * ====================================================================== */

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
            /* pivô ~ 0: NÃO sobrescreve x[linha]; apenas marca e segue */
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
 * WRAPPER (decide status só no final)
 * ====================================================================== */

GaussStatus gauss(double** matrizEstendida, int ordemMatriz,
                  double* vetorSolucao, double tolerancia) {
    gauss_reset_flags();

    (void) eliminacao_parcial(matrizEstendida, ordemMatriz, tolerancia);
    (void) substituicaoRegressiva(matrizEstendida, ordemMatriz, vetorSolucao, tolerancia);

    if (g_gauss_inconsistente)           return GAUSS_INCONSISTENTE;
    if (g_gauss_pivo_quase_zero)         return GAUSS_SINGULAR;
    return GAUSS_OK;
}

/* ======================================================================
 * MENSAGEM DE STATUS
 * ====================================================================== */

void imprimirStatus(GaussStatus status) {
    if (status == GAUSS_OK) {
        puts("OK");
    } else if (status == GAUSS_SINGULAR) {
        puts("Sistema singular ou numericamente instavel (pivo ~ 0).");
    } else if (status == GAUSS_INCONSISTENTE) {
        puts("Sistema inconsistente (linha zero em A com b != 0).");
    }
}
