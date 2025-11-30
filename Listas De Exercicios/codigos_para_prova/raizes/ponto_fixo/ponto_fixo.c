/* ponto_fixo.c */
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "ponto_fixo.h"

/* =============== DEFINA SUA FUNÇÃO g(x) AQUI ================== */
/* Exemplo clássico: resolver x = cos(x), raiz ~ 0.739085...     */
double g(double x) {
    return exp(-x);             
}
/* ============================================================= */

/* f(x) = g(x) - x, só pra avaliar o erro na "raiz" */
static double f(double x) {
    return g(x) - x;
}

double ponto_fixo(double x0, double tol_percent, int max_iter, int verbose) {
    double xk = x0;
    double x_next = x0;
    double erro = 100.0;

    clock_t t0 = clock();

    int k;
    for (k = 1; k <= max_iter; k++) {
        x_next = g(xk);

        if (k == 1) {
            erro = 100.0;
        } else {
            erro = fabs((x_next - xk) / (x_next == 0.0 ? 1.0 : x_next)) * 100.0;
        }

        if (verbose) {
            printf("it=%3d | xk=%.10f xk+1=%.10f g(xk)=%.10f err=%.6f%%\n",
                   k, xk, x_next, g(xk), erro);
        }

        /* critério de parada:
           - erro relativo entre iterações < tol_percent (ex: 5 %)
        */
        if (erro < tol_percent) {
            break;
        }

        xk = x_next;
    }

    clock_t t1 = clock();
    double tempo_ms = 1000.0 * (double)(t1 - t0) / CLOCKS_PER_SEC;

    printf("\nTempo de execucao: %.6f ms\n", tempo_ms);
    if (k > max_iter) {
        printf("ATENCAO: Atingiu max_iter sem satisfazer o criterio de parada.\n");
    } else {
        printf("Iteracoes realizadas: %d\n", k);
    }

    return x_next;
}
