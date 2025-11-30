/* secante.c */
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "secante.h"

/* ================== DEFINA SUA FUNÇÃO AQUI ================== */
double f(double x) {
    // EXEMPLO: resolver exp(-x) - x = 0
    return exp(-x) - x;

    // Você pode trocar por qualquer função:
    // return sin(x) - x/2.0;
    // return x*x - 2.0;
}
/* ============================================================ */

double secante(double x0, double x1, double tol_percent, int max_iter, int verbose) {
    double f0 = f(x0);
    double f1 = f(x1);

    if (fabs(f0) < 1e-15) {
        printf("Aviso: f(x0) ja eh muito proximo de zero.\n");
        return x0;
    }
    if (fabs(f1) < 1e-15) {
        printf("Aviso: f(x1) ja eh muito proximo de zero.\n");
        return x1;
    }

    double x2 = x1;
    double erro = 100.0;

    clock_t t0 = clock();

    for (int k = 1; k <= max_iter; k++) {

        double denom = (f1 - f0);
        if (fabs(denom) < 1e-15) {
            printf("ERRO: denominador muito pequeno em k=%d (f1 - f0 ~ 0).\n", k);
            break;
        }

        x2 = x1 - f1 * (x1 - x0) / denom;
        double f2 = f(x2);

        if (k == 1) {
            erro = 100.0;
        } else {
            if (x2 != 0.0)
                erro = fabs((x2 - x1) / x2) * 100.0;
            else
                erro = fabs(x2 - x1) * 100.0; // caso raro, só pra evitar div 0
        }

        if (verbose) {
            printf("it=%3d | x0=%lf x1=%lf x2=%lf f(x2)=%.6e err=%.6f%%\n",
                   k, x0, x1, x2, f2, erro);
        }

        // critério de parada: erro relativo em % menor que tol_percent
        if (k > 1 && erro < tol_percent) {
            break;
        }

        // Atualiza para próxima iteração
        x0 = x1;
        f0 = f1;
        x1 = x2;
        f1 = f2;
    }

    clock_t t1 = clock();
    double tempo_ms = 1000.0 * (double)(t1 - t0) / CLOCKS_PER_SEC;

    printf("\nTempo de execucao: %.6f ms\n", tempo_ms);

    return x2;
}
