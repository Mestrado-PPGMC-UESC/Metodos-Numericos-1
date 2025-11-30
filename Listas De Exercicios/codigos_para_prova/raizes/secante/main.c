/* main.c */
#include <stdio.h>
#include <math.h>
#include "secante.h"

int main(void) {
    double x0, x1;
    double tol_percent;
    int max_iter;
    int verbose;

    printf("Metodo da Secante\n");
    printf("Funcao f(x) definida em secante.c\n\n");

    printf("Digite x0 (primeiro chute): ");
    if (scanf("%lf", &x0) != 1) {
        printf("Entrada invalida.\n");
        return 1;
    }

    printf("Digite x1 (segundo chute): ");
    if (scanf("%lf", &x1) != 1) {
        printf("Entrada invalida.\n");
        return 1;
    }

    printf("Digite a tolerancia em porcentagem (ex: 5 para 5%%): ");
    if (scanf("%lf", &tol_percent) != 1 || tol_percent <= 0.0) {
        printf("Entrada invalida para tolerancia.\n");
        return 1;
    }

    printf("Digite o numero maximo de iteracoes: ");
    if (scanf("%d", &max_iter) != 1 || max_iter <= 0) {
        printf("Entrada invalida para max_iter.\n");
        return 1;
    }

    printf("Modo detalhado? (0 = nao, 1 = sim): ");
    if (scanf("%d", &verbose) != 1 || (verbose != 0 && verbose != 1)) {
        printf("Entrada invalida para verbose.\n");
        return 1;
    }

    printf("\nParametros:\n");
    printf("  x0 = %.6f\n", x0);
    printf("  x1 = %.6f\n", x1);
    printf("  tol = %.3f%%%%\n", tol_percent);
    printf("  max_iter = %d\n", max_iter);
    printf("  verbose = %d\n\n", verbose);

    double raiz = secante(x0, x1, tol_percent, max_iter, verbose);
    double fres = f(raiz);

    printf("\nRaiz aproximada: %.10f\n", raiz);
    printf("f(raiz) = %.6e\n", fres);

    return 0;
}
