/* main.c */
#include <stdio.h>
#include <math.h>
#include "newton_raphson.h"

int main(void) {
    double x0;
    double tol_percent;
    int max_iter;
    int verbose;

    printf("Metodo de Newton-Raphson\n");
    printf("Funcao f(x) definida em newton_raphson.c\n\n");

    printf("Digite o chute inicial x0: ");
    if (scanf("%lf", &x0) != 1) {
        printf("Entrada invalida para x0.\n");
        return 1;
    }

    printf("Digite a tolerancia (em %), ex: 5 para 5%%: ");
    if (scanf("%lf", &tol_percent) != 1) {
        printf("Entrada invalida para tolerancia.\n");
        return 1;
    }

    printf("Digite o numero maximo de iteracoes: ");
    if (scanf("%d", &max_iter) != 1) {
        printf("Entrada invalida para max_iter.\n");
        return 1;
    }

    printf("Verbose? (0 = nao, 1 = sim): ");
    if (scanf("%d", &verbose) != 1) {
        printf("Entrada invalida para verbose.\n");
        return 1;
    }

    if (tol_percent <= 0.0) {
        printf("Erro: tol_percent deve ser > 0.\n");
        return 1;
    }
    if (max_iter <= 0) {
        printf("Erro: max_iter deve ser > 0.\n");
        return 1;
    }

    printf("\nParametros:\n");
    printf("  x0        = %.10f\n", x0);
    printf("  tol       = %.3f%%\n", tol_percent);
    printf("  max_iter  = %d\n", max_iter);
    printf("  verbose   = %d\n\n", verbose);

    double raiz = newton_raphson(x0, tol_percent, max_iter, verbose);
    double f_raiz = f(raiz);

    printf("\nRaiz aproximada: %.10f\n", raiz);
    printf("f(raiz) = %.6e\n", f_raiz);

    return 0;
}
