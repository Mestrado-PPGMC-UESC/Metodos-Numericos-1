from utils import ListaPontosND
import numpy as np


def get_a_matrix(lista_pontos: ListaPontosND) -> np.ndarray:
    """
    Constrói a matriz X^T * X.
    O índice 0 representa o intercepto (coluna de 1s).
    """
    dimensao = lista_pontos.dimension
    matrix = np.zeros((dimensao, dimensao))
    n = lista_pontos.size
    
    # indice 0  => tudo 1
    # indice 1 => variaveis indenpendente correspondente
    colunas_x = [np.ones(n)] 
    num_preditores = dimensao - 1 # exclui y
    for i in range(num_preditores):
        colunas_x.append(np.array(lista_pontos.get_index_coordinate_values(i)))

    # preenche a matriz simétrica
    for i in range(dimensao):
        for j in range(i, dimensao):
            soma = np.dot(colunas_x[i], colunas_x[j])
            matrix[i, j] = soma
            matrix[j, i] = soma 

    return matrix


def get_b_vector(lista_postos: ListaPontosND) -> np.ndarray:
    n = lista_postos.dimension
    vector = np.zeros((n, 1))
    y_values = np.array(lista_postos.get_index_coordinate_values(-1))
    for i in range(n):
        if i == 0:
            vector[i][0] = np.sum(y_values)
        else:
            x_values = np.array(lista_postos.get_index_coordinate_values(i - 1))
            vector[i][0] = np.sum(y_values * x_values)
    return vector
    

def get_coeficientes(pontos: ListaPontosND) -> list[float]:
    if pontos.size < pontos.dimension:
        raise ValueError(f"Número de pontos ({pontos.size}) insuficiente para a dimensão ({pontos.dimension}). Necessário pelo menos {pontos.dimension} pontos.")
    
    a_matrix = get_a_matrix(pontos)
    b_vector = get_b_vector(pontos)
    
    # resolve equação usando numpy
    try:
        coeficientes = np.linalg.solve(a_matrix, b_vector)
        return coeficientes.flatten().tolist()
    except np.linalg.LinAlgError:
        raise ValueError("A matriz singular. Não foi possível calcular a inversa (provavelmente colineariedade perfeita entre variáveis).")