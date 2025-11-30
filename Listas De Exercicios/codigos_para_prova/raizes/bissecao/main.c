#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bissecao.h"

int main(void) {
    double xl, xu, tol;
    int max_iter, verbose;

    printf("=== Metodo da Bissecao ===\n\n");

    printf("Informe xl: ");
    scanf("%lf", &xl);

    printf("Informe xu: ");
    scanf("%lf", &xu);

    printf("Informe a tolerancia (ex: 1e-6): ");
    scanf("%lf", &tol);

    printf("Informe max_iter: ");
    scanf("%d", &max_iter);

    printf("Verbose? (1=sim, 0=nao): ");
    scanf("%d", &verbose);

    printf("\nExecutando...\n");

    double raiz = bissecao(xl, xu, tol, max_iter, verbose);

    if (!isnan(raiz)) {
        printf("\nRaiz aproximada: %.10f\n", raiz);
        printf("f(raiz) = %.6e\n", f(raiz));
    } else {
        printf("\nNao foi possivel encontrar raiz no intervalo informado.\n");
    }

    return 0;
}
