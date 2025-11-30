#ifndef DECOMP_LU_H
#define DECOMP_LU_H

/**
 * @file decomp_lu.h
 * @brief Rotinas de decomposição LU (sem pivotamento) e funções auxiliares
 *        para substituições progressiva e regressiva, além de montagem da inversa.
 *
 * Implementação baseada no método de Doolittle (diag(L) = 1.0).
 * Todas as funções assumem que as matrizes são quadradas e
 * alocadas dinamicamente como `double**` com tamanho [ordemMatriz][ordemMatriz].
 *
 * @date 2025
 * @author João
 */

 /**
  * @enum LUStatus
  * @brief Indica o status do processo de fatoração LU.
  */
typedef enum {
    LU_OK = 0,        /**< Fatoração concluída com sucesso. */
    LU_PIVO_NULO = 1  /**< Foi detectado pivô nulo ou quase nulo. */
} LUStatus;

/**
 * @brief Realiza a fatoração LU (Doolittle, sem pivotamento): A = L * U.
 *        L é triangular inferior com diagonal 1.0; U é triangular superior.
 *
 * @param matrizA        Matriz de entrada (n x n) — NÃO é modificada.
 * @param ordemMatriz    Ordem n da matriz.
 * @param matrizL        Saída: matriz L (n x n), triangular inferior (diag = 1.0).
 * @param matrizU        Saída: matriz U (n x n), triangular superior.
 * @param tolerancia     Valor mínimo aceito para o pivô (|pivô| < tolerancia => aviso).
 * @return LU_OK se sucesso; LU_PIVO_NULO se detectado pivô nulo/quase nulo.
 */
LUStatus luDecomposicao(double** matrizA,
                        int ordemMatriz,
                        double** matrizL,
                        double** matrizU,
                        double tolerancia);

/**
 * @brief Resolve o sistema L * y = b (substituição progressiva).
 *
 * @param matrizL        Matriz triangular inferior (n x n).
 * @param vetorB         Vetor do lado direito b (n x 1).
 * @param vetorY         Vetor solução y (n x 1).
 * @param ordemMatriz    Ordem n da matriz.
 * @param tolerancia     Valor mínimo para elementos da diagonal.
 * @return Sempre 0 (mantém compatibilidade com outras versões).
 */
int substituicaoProgressiva(double** matrizL,
                            const double* vetorB,
                            double* vetorY,
                            int ordemMatriz,
                            double tolerancia);

/**
 * @brief Resolve o sistema U * x = y (substituição regressiva).
 *
 * @param matrizU        Matriz triangular superior (n x n).
 * @param vetorY         Vetor do lado direito y (n x 1).
 * @param vetorX         Vetor solução x (n x 1).
 * @param ordemMatriz    Ordem n da matriz.
 * @param tolerancia     Valor mínimo para elementos da diagonal.
 * @return Sempre 0 (mantém compatibilidade com outras versões).
 */
int substituicaoRegressiva_U(double** matrizU,
                             const double* vetorY,
                             double* vetorX,
                             int ordemMatriz,
                             double tolerancia);

/**
 * @brief Preenche uma matriz identidade de ordem n.
 *
 * @param matrizI        Matriz a ser preenchida (n x n).
 * @param ordemMatriz    Ordem n da matriz.
 */
void preencherIdentidade(double** matrizI, int ordemMatriz);

/**
 * @brief Monta a matriz inversa A⁻¹ a partir de L e U já fatoradas.
 *
 * Resolve sucessivamente Ly = e_j e Ux = y para cada coluna da identidade.
 *
 * @param matrizL        Matriz triangular inferior (n x n).
 * @param matrizU        Matriz triangular superior (n x n).
 * @param matrizInversa  Saída: matriz inversa (n x n).
 * @param ordemMatriz    Ordem n da matriz.
 * @param tolerancia     Valor mínimo para pivôs.
 * @return 0 se sucesso; código > 0 em falhas de alocação ou instabilidade.
 */
int inversaAPartirDeLU(double** matrizL,
                       double** matrizU,
                       double** matrizInversa,
                       int ordemMatriz,
                       double tolerancia);

/**
 * @brief Fatora A em L e U e monta diretamente a inversa.
 *
 * Combina internamente `luDecomposicao()` e `inversaAPartirDeLU()`.
 *
 * @param matrizA        Matriz de entrada (n x n) — modificada internamente.
 * @param ordemMatriz    Ordem n da matriz.
 * @param tolerancia     Valor mínimo aceito para pivôs.
 * @param matrizInversa  Saída: matriz inversa (n x n).
 * @return 0 se sucesso; código > 0 em falhas de fatoração ou alocação.
 */
int inversaPorLU(double** matrizA,
                 int ordemMatriz,
                 double tolerancia,
                 double** matrizInversa);

/**
 * @brief Redefine os avisos internos da LU (pivôs quase nulos).
 *
 * Chame antes de iniciar uma nova fatoração para limpar o estado.
 */
void luRedefinirAvisos();

/**
 * @brief Verifica se algum pivô (ou elemento diagonal) foi considerado ~0.
 *
 * @return 1 se pivô ~ 0 foi detectado; 0 caso contrário.
 */
int luFlagPivoQuaseZero();

#endif /* DECOMP_LU_H */
