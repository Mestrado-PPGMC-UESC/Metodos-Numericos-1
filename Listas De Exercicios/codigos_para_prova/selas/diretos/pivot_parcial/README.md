# Método de Gauss com Pivotamento Parcial

Este diretório contém a implementação do **método de Gauss com pivotamento parcial**, que realiza trocas de linha para aumentar a estabilidade numérica do processo de eliminação.  
O código segue o mesmo padrão do módulo `gauss_common`, utilizando **flags de diagnóstico** e um comportamento *fail-soft*: o programa não aborta em pivôs pequenos, apenas sinaliza a ocorrência.

## Estrutura dos Arquivos

- **main.c**: Programa principal que gera o sistema de teste (matriz de Hilbert), executa o método de Gauss com pivotamento parcial, mede o tempo de execução e exibe os resultados.
- **gauss.c / gauss.h**: Implementação do algoritmo de eliminação de Gauss com pivotamento parcial e da substituição regressiva, incluindo flags de diagnóstico e contagem de trocas de linha.
- **utils.c / utils.h**: Funções auxiliares para criação e liberação de matrizes, cálculo de erro relativo e geração da matriz de Hilbert aumentada.

## Funcionamento do Método

O método de Gauss com pivotamento parcial busca corrigir as instabilidades numéricas do método sem pivotamento, realizando trocas de linhas antes de cada etapa de eliminação.

### Etapas principais:
1. **Seleção do pivô**: em cada coluna *k*, escolhe-se a linha com o maior valor absoluto em `|A[i,k]|` e realiza-se a troca com a linha atual.
2. **Eliminação direta**: zera os elementos abaixo do pivô, transformando a matriz [A|b] em uma forma triangular superior [U|c].
3. **Substituição regressiva**: resolve o sistema triangular superior resultante, obtendo o vetor solução *x*.
4. **Verificação de inconsistências**: detecta linhas nulas com termos independentes não nulos (sistemas impossíveis).
5. **Flags de diagnóstico**: marca ocorrência de pivôs muito pequenos e contabiliza trocas de linhas.

## Política *Fail-Soft* e Diagnóstico

O algoritmo **não interrompe** a execução em caso de pivôs pequenos.  
Essas ocorrências são registradas por meio das flags globais:

| Situação detectada | Efeito | Função de diagnóstico |
|--------------------|---------|------------------------|
| Pivô pequeno (`|pivô| < tolerancia`) | Marca flag, continua execução | `gaussFlagPivoQuaseZero()` |
| Troca de linhas | Incrementa contador global | `gaussContagemTrocasLinhas()` |
| Linha 0…0 \| b≠0 | Interrompe execução | Retorna `GAUSS_INCONSISTENTE` |

## Como Compilar e Executar

### Compilação
No Windows (MinGW):
```bash
gcc main.c gauss.c utils.c -o gauss_pivot.exe -lm
```

No Linux:
```bash
gcc main.c gauss.c utils.c -lm -o gauss_pivot
```

### Execução
```bash
./gauss_pivot
```
ou
```bash
./gauss_pivot.exe
```

## Status de Retorno

| Código | Significado |
|--------|--------------|
| **GAUSS_OK** | Execução normal (sem contradições) |
| **GAUSS_SINGULAR** | Sistema numericamente instável (houve pivôs ≈ 0) |
| **GAUSS_INCONSISTENTE** | Sistema inconsistente (linha nula com b≠0) |
