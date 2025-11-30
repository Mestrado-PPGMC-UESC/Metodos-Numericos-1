#include <stdio.h>
#include <math.h>
#include <time.h>
#include "bissecao.h"

/* ================== DEFINA SUA FUNÇÃO AQUI ================== */
double f(double x) {
    return exp(-x) - x;
}
/* ============================================================= */


double bissecao(double xl, double xu, double tol_percent, int max_iter, int verbose) {

    double fl = f(xl);
    double fu = f(xu);

    if (fl * fu > 0) {
        printf("ERRO: intervalo invalido. f(xl) e f(xu) tem o mesmo sinal.\n");
        return NAN;
    }

    double xi = 0.0, xi_ant = 0.0, fi = 0.0;
    double erro = 100.0;   // começa em 100%
    int k;

    clock_t t0 = clock();

    for (k = 1; k <= max_iter; k++) {

        xi_ant = xi;      
        xi = (xl + xu) / 2.0;
        fi = f(xi);

        if (k == 1) {
            erro = 100.0;    // primeira iteração não tem erro relativo
        } else {
            erro = fabs((xi - xi_ant) / xi) * 100.0;
        }

        if (verbose) {
            printf("it=%3d | xl=%.6lf xu=%.6lf xi=%.6lf f(xi)=%.6e err=%.6f%%\n",
                   k, xl, xu, xi, fi, erro);
        }

        // critério correto:
        if (erro < tol_percent)
            break;

        // bisseção normal:
        if (fl * fi < 0) {
            xu = xi;
            fu = fi;
        } else {
            xl = xi;
            fl = fi;
        }
    }

    clock_t t1 = clock();
    double tempo_ms = 1000.0 * (double)(t1 - t0) / CLOCKS_PER_SEC;

    printf("\n>>> Última iteração:\n");
    printf("it=%3d | xl=%.6lf xu=%.6lf xi=%.6lf f(xi)=%.6e err=%.6f%%\n",
           k, xl, xu, xi, fi, erro);

    printf("\nTempo de execucao: %.6f ms\n", tempo_ms);

    return xi;
}
