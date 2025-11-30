#ifndef UTILS_H
#define UTILS_H

/**
 * @brief Imprime a matriz estendida [A|b] no console.
 *
 * Cada linha contém os coeficientes da matriz A seguidos do termo independente b.
 *
 * @param matrizEstendida Ponteiro para a matriz estendida [A|b].
 * @param ordemMatriz Ordem da matriz quadrada A.
 */
void imprimirMatriz(double** matrizEstendida, int ordemMatriz);

/**
 * @brief Libera a memória alocada para a matriz estendida [A|b].
 *
 * @param matrizEstendida Ponteiro para a matriz a ser liberada.
 * @param ordemMatriz Ordem da matriz quadrada A.
 */
void liberarMatriz(double** matrizEstendida, int ordemMatriz);

/**
 * @brief Cria e aloca o vetor solução x.
 *
 * @param ordemMatriz Dimensão do sistema (n).
 * @return Ponteiro para o vetor solução alocado dinamicamente.
 */
double* criarVetorSolucao(int ordemMatriz);

/**
 * @brief Libera a memória ocupada pelo vetor solução.
 *
 * @param vetorSolucao Ponteiro para o vetor solução a ser liberado.
 */
void liberarVetorSolucao(double* vetorSolucao);

/**
 * @brief Imprime no console o vetor solução x.
 *
 * Mostra cada componente do vetor em notação científica com alta precisão.
 *
 * @param vetorSolucao Ponteiro para o vetor solução.
 * @param ordemMatriz Dimensão do sistema (n).
 */
void imprimirSolucao(const double* vetorSolucao, int ordemMatriz);

/**
 * @brief Libera a matriz estendida [A|b] e o vetor solução em uma única chamada.
 *
 * @param matrizEstendida Ponteiro para a matriz estendida a ser liberada.
 * @param ordemMatriz Ordem da matriz quadrada A.
 * @param vetorSolucao Ponteiro para o vetor solução a ser liberado.
 */
void liberar(double** matrizEstendida, int ordemMatriz, double* vetorSolucao);

/**
 * @brief Gera a matriz de Hilbert aumentada [A|b] de ordem n.
 *
 * A matriz A é definida como:
 *   A_ij = 1 / (i + j - 1), para i,j iniciando em 1.
 * O vetor b é a soma dos elementos de cada linha de A.
 *
 * @param ordemMatriz Ordem da matriz (n).
 * @return Ponteiro para a matriz estendida [A|b].
 */
double** gerarHilbertAumentada(int ordemMatriz);

/**
 * @brief Calcula e imprime os erros relativos entre a solução obtida e a solução exata (1,...,1)^T.
 *
 * Para cada componente x_i, calcula:
 *   erro_i = |x_i - 1| / |1| * 100%
 *
 * Também imprime o erro médio e o erro máximo.
 *
 * @param vetorSolucao Ponteiro para o vetor solução aproximada.
 * @param ordemMatriz Dimensão do sistema (n).double** lerMatrizAumentadaDeArquivo(const char* nomeArquivo, int* ordemOut) {

 */
void calcularErroRelativo(const double* vetorSolucao, int ordemMatriz);

double** lerMatrizAumentadaDeArquivo(const char* nomeArquivo, int* ordemOut);



#endif
