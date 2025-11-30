/* main.c */
#include <stdio.h>
#include <math.h>
#include "ponto_fixo.h"

int main(void) {
    double x0, tol_percent;
    int max_iter, verbose;

    printf("=========== MÉTODO DO PONTO FIXO ===========\n");
    printf("A funcao g(x) deve ser definida em ponto_fixo.c\n\n");

    printf("Digite o chute inicial x0: ");
    scanf("%lf", &x0);

    printf("Digite a tolerancia em %% (ex: 5 para 5%%): ");
    scanf("%lf", &tol_percent);

    printf("Digite o numero maximo de iteracoes: ");
    scanf("%d", &max_iter);

    printf("Mostrar iteracoes? (1 = sim, 0 = nao): ");
    scanf("%d", &verbose);

    printf("\nExecutando Ponto Fixo...\n");

    double raiz = ponto_fixo(x0, tol_percent, max_iter, verbose);

    printf("\n=============================================\n");
    printf("Raiz aproximada: %.10f\n", raiz);
    printf("g(raiz) - raiz = %.6e\n", g(raiz) - raiz);
    printf("=============================================\n");

    return 0;
}
