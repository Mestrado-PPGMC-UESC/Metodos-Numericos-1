/* newton_raphson.c */
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "newton_raphson.h"

/* ================== DEFINA SUA FUNÇÃO AQUI ================== */
/* Exemplo: f(x) = e^{-x} - x */
double f(double x) {
    return exp(-x) - x;
}

/* Derivada: f'(x) = -e^{-x} - 1 */
double df(double x) {
    return -exp(-x) - 1.0;
}
/* ============================================================ */

double newton_raphson(double x0, double tol_percent, int max_iter, int verbose) {
    double x_atual = x0;
    double x_ant   = x0;
    double fx, dfx, fx_atual;
    double erro = 100.0;

    clock_t t0 = clock();

    int k;
    for (k = 1; k <= max_iter; k++) {
        fx  = f(x_atual);
        dfx = df(x_atual);

        if (dfx == 0.0) {
            printf("ERRO: f'(x) = 0 em x = %.10f. Método travou.\n", x_atual);
            break;
        }

        x_ant   = x_atual;
        x_atual = x_atual - fx / dfx;
        fx_atual = f(x_atual);

        if (k == 1) {
            erro = 100.0;  // erro "infinito" na 1ª iteração
        } else {
            erro = fabs((x_atual - x_ant) / x_atual) * 100.0;
        }

        if (verbose) {
            printf("it=%3d | x=%.10f f(x)=%.6e err=%.6f%%\n",
                   k, x_atual, fx_atual, erro);
        }

        /* critério de parada:
           - erro relativo (%) < tol_percent
           - ou |f(x)| muito pequeno (opcional, bem rígido)
        */
        if (erro < tol_percent || fabs(fx_atual) < 1e-12) {
            break;
        }
    }

    clock_t t1 = clock();
    double tempo_ms = 1000.0 * (double)(t1 - t0) / CLOCKS_PER_SEC;

    printf("\nTempo de execucao: %.6f ms\n", tempo_ms);

    if (k > max_iter) {
        printf("(ATENCAO) Atingiu max_iter sem satisfazer os criterios de parada.\n");
    }

    return x_atual;
}
