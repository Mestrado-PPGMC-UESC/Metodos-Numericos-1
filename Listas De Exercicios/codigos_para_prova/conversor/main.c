#include <stdio.h>
#include <string.h>
#include "conversor.h"

int main(void) {
    int opcao;
    char bin[256];
    char resultado[256];

    do {
        printf("\n=============================\n");
        printf("  CONVERSOR BINÁRIO/DECIMAL\n");
        printf("=============================\n");
        printf("1 - Decimal inteiro          -> Binário (string)\n");
        printf("2 - Binário inteiro (string) -> Decimal inteiro\n");
        printf("3 - Decimal (double)         -> Binário float genérico\n");
        printf("4 - Binário float genérico   -> Decimal (double)\n");
        printf("0 - Sair\n");
        printf("Opção: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida.\n");
            limpaBufferEntrada();
            continue;
        }

        limpaBufferEntrada();

        if (opcao == 1) {
            long long n;
            printf("Digite o inteiro: ");
            scanf("%lld", &n);
            limpaBufferEntrada();
            decimalIntParaBinario(n, resultado, sizeof(resultado));
            printf("Binário: %s\n", resultado);

        } else if (opcao == 2) {
            printf("Digite binário: ");
            fgets(bin, sizeof(bin), stdin);
            bin[strcspn(bin, "\n")] = '\0';
            printf("Decimal: %lld\n", binarioParaDecimalInt(bin));

        } else if (opcao == 3) {
            double x;
            int totalBits, bitsExpMod;
            printf("Decimal: ");
            scanf("%lf", &x);
            printf("Total de bits: ");
            scanf("%d", &totalBits);
            printf("Bits do módulo do expoente: ");
            scanf("%d", &bitsExpMod);
            limpaBufferEntrada();

            doubleParaBinarioFloatGenerico(x, totalBits, bitsExpMod, resultado);
            printf("Binário: %s\n", resultado);

        } else if (opcao == 4) {
            int totalBits, bitsExpMod;
            printf("Total de bits: ");
            scanf("%d", &totalBits);
            printf("Bits do módulo do expoente: ");
            scanf("%d", &bitsExpMod);
            limpaBufferEntrada();

            printf("Binário: ");
            fgets(bin, sizeof(bin), stdin);
            bin[strcspn(bin, "\n")] = '\0';

            double v = binarioFloatGenerico(bin, totalBits, bitsExpMod);
            printf("Decimal: %.15g\n", v);
        }

    } while (opcao != 0);

    return 0;
}
