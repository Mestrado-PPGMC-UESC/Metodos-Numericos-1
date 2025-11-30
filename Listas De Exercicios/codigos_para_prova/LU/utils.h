/**
 * @file utils.h
 * @brief Funções utilitárias para manipulação de matrizes e operações numéricas básicas.
 *
 * Este módulo provê funções auxiliares para alocação, liberação, clonagem,
 * impressão e multiplicação de matrizes quadradas, além de utilitários para
 * cálculo de erros e geração de matrizes de teste (A1, A2).
 *
 * Todas as funções assumem matrizes alocadas dinamicamente na forma de
 * ponteiros duplos (`double**`) e quadradas de ordem `n`.
 */

#ifndef UTILS_H
#define UTILS_H

/**
 * @brief Aloca dinamicamente uma matriz quadrada n×n de números em ponto flutuante.
 * @param ordemMatriz Ordem da matriz (n).
 * @return Ponteiro para a matriz alocada.
 */
double** alocarMatriz(int ordemMatriz);

/**
 * @brief Libera a memória ocupada por uma matriz previamente alocada.
 * @param matriz Ponteiro para a matriz a ser liberada.
 * @param ordemMatriz Ordem da matriz (n).
 */
void liberarMatriz(double** matriz, int ordemMatriz);

/**
 * @brief Imprime a matriz formatada com um título opcional.
 * @param titulo Título a ser exibido antes da matriz.
 * @param matriz Ponteiro para a matriz a ser impressa.
 * @param ordemMatriz Ordem da matriz (n).
 */
void imprimirMatriz(const char* titulo, double** matriz, int ordemMatriz);

/**
 * @brief Copia o conteúdo de uma matriz origem para uma matriz destino.
 * @param destino Matriz de destino.
 * @param origem Matriz de origem.
 * @param ordemMatriz Ordem da matriz (n).
 */
void copiarMatriz(double** destino, double** origem, int ordemMatriz);

/**
 * @brief Multiplica duas matrizes quadradas A e B, armazenando o resultado em matrizResultado.
 * @param matrizA Primeira matriz (A).
 * @param matrizB Segunda matriz (B).
 * @param matrizResultado Matriz que armazenará o resultado da multiplicação.
 * @param ordemMatriz Ordem da matriz (n).
 */
void multiplicarMatrizes(double** matrizA, double** matrizB, double** matrizResultado, int ordemMatriz);

/**
 * @brief Calcula a diferença entre uma matriz e a identidade (erro absoluto elemento a elemento).
 * @param matriz Matriz de entrada.
 * @param ordemMatriz Ordem da matriz (n).
 * @param matrizErros Matriz de saída contendo os erros absolutos.
 */
void calcularErrosIdentidade(double** matriz, int ordemMatriz, double** matrizErros);

/**
 * @brief Calcula o erro absoluto elemento a elemento e retorna o erro máximo.
 *
 * Variante que, além de preencher a matriz de erros, retorna o maior erro
 * absoluto encontrado — útil para análise rápida da precisão numérica.
 *
 * @param matriz Matriz de entrada.
 * @param ordemMatriz Ordem da matriz (n).
 * @param matrizErros Matriz de saída contendo os erros absolutos.
 * @return O maior erro absoluto encontrado.
 */
double calcularErrosIdentidadeComMax(double** matriz, int ordemMatriz, double** matrizErros);

/**
 * @brief Cria uma cópia independente de uma matriz quadrada n×n.
 * @param matrizOriginal Matriz de origem a ser clonada.
 * @param tamanhoMatriz Ordem da matriz (n).
 * @return Nova matriz alocada contendo uma cópia dos elementos da original.
 */
double** clonarMatriz(double** matrizOriginal, int tamanhoMatriz);

/**
 * @brief Cria e retorna a matriz A1 utilizada nos experimentos.
 * @return Ponteiro para a matriz A1.
 */
double** criarMatrizA1();

/**
 * @brief Cria e retorna a matriz A2 utilizada nos experimentos.
 * @return Ponteiro para a matriz A2.
 */
double** criarMatrizA2();

#endif /* UTILS_H */
