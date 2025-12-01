from ponto2D import ListaPontos2D
from minimos_quadrados import get_coeficientes
import sympy as sp

def transform(values: list[float], funcao: sp.Expr) -> list[float]:
    """Aplica a transformação """
    x = sp.symbols('x')
    resultados = []
    for v in values:
        valor_avaliado = funcao.subs(x, v)
        resultados.append(float(valor_avaliado.evalf()))
    return resultados

def exponencial_regression(pontos: ListaPontos2D) -> list[float]:
    # Função = alfa * e^(beta * x)
    # Transformação: ln(y) = ln(alfa) + beta*x
    y_values = pontos.get_y_values()
    transformed_y = transform(y_values, sp.log(sp.symbols('x')))
    new_pontos = ListaPontos2D.from_x_y_values(pontos.get_x_values(), transformed_y)
    coeficientes = get_coeficientes(new_pontos, grau=1)
    alfa = sp.exp(coeficientes[0])
    beta = coeficientes[1]
    return [float(alfa.evalf()), beta]

def potencia_regression(pontos: ListaPontos2D) -> list[float]:
    # Função = alfa * x**beta
    # Transformação: log(y) = log(alfa) + beta*log(x)
    y_values = pontos.get_y_values()
    x_values = pontos.get_x_values()

    transformed_y = transform(y_values, sp.log(sp.symbols('x'), 10))
    transformed_x = transform(x_values, sp.log(sp.symbols('x'), 10))
    new_pontos = ListaPontos2D.from_x_y_values(transformed_x, transformed_y)
    coeficientes = get_coeficientes(new_pontos, grau=1)
    alfa = 10 ** coeficientes[0] 
    beta = coeficientes[1]
    return [float(alfa), float(beta)]

def growth_regression(pontos: ListaPontos2D) -> list[float]:
    # Função = (alfa * x) / (beta + x)
    # Transformação: 1/y = (beta/alfa)*(1/x) + (1/alfa)
    
    y_values = pontos.get_y_values()
    x_values = pontos.get_x_values()
    transformed_y = transform(y_values, 1/sp.symbols('x'))
    transformed_x = transform(x_values, 1/sp.symbols('x'))
    new_pontos = ListaPontos2D.from_x_y_values(transformed_x, transformed_y)
    coeficientes = get_coeficientes(new_pontos, grau=1)
    alfa = 1 / coeficientes[0]
    beta = alfa * coeficientes[1]
    return [float(alfa), float(beta)]
    
    