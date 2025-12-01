from ponto2D import ListaPontos2D
import numpy as np

def linear_regression(pontos: ListaPontos2D) -> list[float]:
    sum_x_multi_y = 0
    sum_x = 0
    sum_y = 0
    sum_x_squared = 0
    n = pontos.size
    for p in pontos.get_pontos():
        sum_x_multi_y += p.x * p.y
        sum_x += p.x
        sum_y += p.y
        sum_x_squared += p.x ** 2
    a1 = (n * sum_x_multi_y - sum_x * sum_y) / (n * sum_x_squared - sum_x ** 2)
    a0 = (sum_y / n) - a1 * (sum_x / n)
    return [a0, a1]

def get_a_matrix(lista_pontos: ListaPontos2D, grau: int) -> np.ndarray:
    matrix = np.zeros((grau + 1, grau + 1))
    x_values = lista_pontos.get_x_values()
    for i in range(grau + 1):
        for j in range(i, grau + 1):
            sum_x_power = 0
            sum_x_power = sum(x ** (i + j) for x in x_values)
            matrix[i][j] = sum_x_power
            matrix[j][i] = sum_x_power
    return matrix


def get_b_vector(lista_pontos: ListaPontos2D, grau: int) -> np.ndarray:
    vector = np.zeros((grau + 1, 1))
    for i in range(grau + 1):
        sum_y_multi_x_power = 0
        for p in lista_pontos.get_pontos():
            sum_y_multi_x_power += p.y * (p.x ** i)
        vector[i][0] = sum_y_multi_x_power
    return vector
    

def get_coeficientes(lista_pontos: ListaPontos2D, grau: int) -> list[float]:
    if lista_pontos.size < grau + 1:
        raise ValueError("Número de pontos insuficiente para o grau do polinômio")
    
    if grau == 1:
        return linear_regression(lista_pontos)
    
    a_matrix = get_a_matrix(lista_pontos, grau)
    b_vector = get_b_vector(lista_pontos, grau)
    
    coeficientes = np.linalg.solve(a_matrix, b_vector)
    return coeficientes.flatten().tolist()