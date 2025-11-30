#include <stdio.h>
#include <math.h>
#include <time.h>
#include "falsa_posicao.h"   // se quiser, pode renomear depois para falsa_posicao.h

/* ================== DEFINA SUA FUNÇÃO AQUI ================== */
double f(double x) {
    // EXEMPLO: f(x) = e^{-x} - x
    return exp(-x) - x;

    // Você pode trocar por qualquer função:
    // return sin(x) - x/2.0;
    // return x*x - 2.0;
}
/* ============================================================= */

/*
 * Método da Falsa Posição (Regula Falsi)
 *
 * xl, xu      -> extremos do intervalo inicial [xl, xu]
 * tol_percent -> tolerância em PORCENTAGEM (%), ex: 5 significa 5%
 * max_iter    -> número máximo de iterações
 * verbose     -> se != 0, imprime cada iteração
 *
 * Retorna a aproximação da raiz.
 */
double falsa_posicao(double xl, double xu, double tol_percent, int max_iter, int verbose) {
    double fl = f(xl);
    double fu = f(xu);

    if (fl * fu > 0.0) {
        printf("ERRO: intervalo invalido. f(xl) e f(xu) tem o mesmo sinal.\n");
        return NAN;
    }

    double xi = 0.0, xi_ant = 0.0, fi = 0.0;
    double erro = 100.0;

    clock_t t0 = clock();

    int k;
    for (k = 1; k <= max_iter; k++) {
        xi_ant = xi;

        /* Fórmula da Falsa Posição:
           xi = xu - fu * (xl - xu) / (fl - fu)
           (equivalente a várias variantes que você vê por aí) */
        xi = xu - fu * (xl - xu) / (fl - fu);
        fi = f(xi);

        if (k == 1) {
            erro = 100.0;   // erro "fake" na primeira iteração
        } else {
            erro = fabs((xi - xi_ant) / (xi == 0.0 ? 1.0 : xi)) * 100.0;
        }

        if (verbose) {
            printf("it=%3d | xl=%lf xu=%lf xi=%lf f(xi)=%.6e err=%.6f%%\n",
                   k, xl, xu, xi, fi, erro);
        }

        /* Critério de parada:
           - ou o erro relativo (%), em relação a xi, fica menor que tol_percent
           - ou |f(xi)| fica absurdamente pequeno (opcional, só pra segurança numérica) */
        if (erro < tol_percent || fabs(fi) < 1e-14) {
            break;
        }

        /* Atualização do intervalo:
           escolhe o subintervalo que mantém a mudança de sinal.
           Na falsa posição, assim como na bisseção, apenas um lado anda. */
        if (fl * fi < 0.0) {
            xu = xi;
            fu = fi;
        } else {
            xl = xi;
            fl = fi;
        }
    }

    clock_t t1 = clock();
    double tempo_ms = 1000.0 * (double)(t1 - t0) / CLOCKS_PER_SEC;

    printf("\nTempo de execucao: %.6f ms\n", tempo_ms);

    /* Se você quiser, pode descomentar este bloco para sempre mostrar
       o resumo da última iteração, mesmo com verbose = 0:

    printf("Ultima iteracao: it=%d | xl=%.10f xu=%.10f xi=%.10f f(xi)=%.6e err=%.6f%%\n",
           k, xl, xu, xi, fi, erro);
    */

    return xi;
}
