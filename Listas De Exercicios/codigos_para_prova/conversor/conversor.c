#include <stdio.h>
#include <string.h>
#include <math.h>

void limpaBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* descarta */
    }
}

/*------------------ INTEIRO: DECIMAL -> BINÁRIO ------------------*/
void decimalIntParaBinario(long long n, char *out, size_t tamanho) {
    if (tamanho < 2) {
        if (tamanho > 0) out[0] = '\0';
        return;
    }

    if (n == 0) {
        out[0] = '0';
        out[1] = '\0';
        return;
    }

    int negativo = 0;
    unsigned long long valor;
    if (n < 0) {
        negativo = 1;
        valor = (unsigned long long)(-n);
    } else {
        valor = (unsigned long long)n;
    }

    char temp[128];
    int i = 0;

    while (valor > 0 && i < (int)sizeof(temp) - 1) {
        temp[i++] = (valor % 2) + '0';
        valor /= 2;
    }

    int j = 0;
    if (negativo && j < (int)tamanho - 1) {
        out[j++] = '-';
    }

    // inverte a ordem dos bits
    while (i > 0 && j < (int)tamanho - 1) {
        out[j++] = temp[--i];
    }

    out[j] = '\0';
}

/*------------------ INTEIRO: BINÁRIO -> DECIMAL ------------------*/
long long binarioParaDecimalInt(const char *bin) {
    int sinal = 1;
    size_t i = 0;

    if (bin[0] == '-') {
        sinal = -1;
        i = 1;
    }

    long long valor = 0;

    for (; bin[i] != '\0'; ++i) {
        if (bin[i] == '0' || bin[i] == '1') {
            valor = valor * 2 + (bin[i] - '0');
        }
        // caracteres diferentes de 0/1 são simplesmente ignorados
    }

    return sinal * valor;
}

/*------------------ PONTO FLUTUANTE GENÉRICO: BINÁRIO -> DECIMAL ------------------*/
/*
   Formato genérico de totalBits bits:
   [0]   = sinal do número
   [1]   = sinal do expoente (0 = +, 1 = -)
   [2..(1+bitsExpMod)] = módulo do expoente
   [restante] = mantissa (inteiro), interpretada como mantissa / 2^{bitsMantissa}
*/
double binarioFloatGenerico(const char *bin, int totalBits, int bitsExpMod) {
    if ((int)strlen(bin) != totalBits) {
        return NAN;  /* erro: tamanho não bate */
    }

    /* sinal do número */
    int sinalNum = (bin[0] == '1');

    /* sinal do expoente */
    int sinalExp = (bin[1] == '1');

    /* módulo do expoente */
    int expMod = 0;
    for (int i = 0; i < bitsExpMod; i++) {
        char c = bin[2 + i];
        if (c != '0' && c != '1') return NAN;
        expMod = (expMod << 1) | (c - '0');
    }

    int expoente = sinalExp ? -expMod : expMod;

    /* bits restantes são da mantissa */
    int bitsMantissa = totalBits - 2 - bitsExpMod;
    if (bitsMantissa <= 0) {
        return NAN; /* formato impossível */
    }

    int mantissa = 0;
    for (int i = 0; i < bitsMantissa; i++) {
        char c = bin[2 + bitsExpMod + i];
        if (c != '0' && c != '1') return NAN;
        mantissa = (mantissa << 1) | (c - '0');
    }

    if (mantissa == 0)
        return 0.0;

    double escalaMantissa = pow(2.0, bitsMantissa);
    double frac = (double) mantissa / escalaMantissa;

    /* aplica expoente */
    double valor = frac * pow(2.0, expoente);

    if (sinalNum)
        valor = -valor;

    return valor;
}

/*------------------ PONTO FLUTUANTE GENÉRICO: DECIMAL -> BINÁRIO ------------------*/
/*
   Converte double x para o formato genérico:
   totalBits: total de bits do número
   bitsExpMod: número de bits para o módulo do expoente
   Convenção igual à de cima.
*/
void doubleParaBinarioFloatGenerico(
        double x,
        int totalBits,
        int bitsExpMod,
        char *out
) {
    if (totalBits <= 2 + bitsExpMod) {
        /* não cabe mantissa */
        out[0] = '\0';
        return;
    }

    if (x == 0.0) {
        for (int i = 0; i < totalBits; i++) {
            out[i] = '0';
        }
        out[totalBits] = '\0';
        return;
    }

    int sinalNum = (x < 0.0);
    double y = sinalNum ? -x : x;

    int bitsMantissa = totalBits - 2 - bitsExpMod;

    double melhorErro = -1.0;
    int melhorE = 0;
    int melhorM = 0;

    /* expoentes possíveis vão de -((2^bitsExpMod)-1) até +((2^bitsExpMod)-1) */
    int maxExp = (1 << bitsExpMod) - 1;
    double escalaMantissa = pow(2.0, bitsMantissa);

    for (int E = -maxExp; E <= maxExp; E++) {

        double mReal = y * pow(2.0, -E) * escalaMantissa;
        if (mReal < 0.0)
            continue;

        int M = (int)(mReal + 0.5); /* arredonda */
        if (M < 0 || M > (int)escalaMantissa - 1)
            continue;

        double aproximado = ((double)M / escalaMantissa) * pow(2.0, E);
        double erro = fabs(aproximado - y);

        if (melhorErro < 0.0 || erro < melhorErro) {
            melhorErro = erro;
            melhorE = E;
            melhorM = M;
        }
    }

    /* monta string final */
    int pos = 0;

    /* sinal do número */
    out[pos++] = sinalNum ? '1' : '0';

    /* sinal do expoente */
    out[pos++] = (melhorE < 0) ? '1' : '0';

    /* módulo do expoente */
    int absE = melhorE < 0 ? -melhorE : melhorE;
    if (absE > (1 << bitsExpMod) - 1) {
        absE = (1 << bitsExpMod) - 1; /* saturação, em teoria não deve acontecer */
    }
    for (int i = bitsExpMod - 1; i >= 0; i--) {
        out[pos++] = ((absE >> i) & 1) ? '1' : '0';
    }

    /* mantissa */
    for (int i = bitsMantissa - 1; i >= 0; i--) {
        out[pos++] = ((melhorM >> i) & 1) ? '1' : '0';
    }

    out[pos] = '\0';
}

/*------------------ MAIN / MENU ------------------*/
