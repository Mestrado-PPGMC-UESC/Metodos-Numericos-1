import sympy as sp
from ponto2D import Ponto2D, ListaPontos2D
from pontoND import ListaPontosND

def print_pontos(pontos: list[Ponto2D]) -> None:
    for p in pontos:
        print(f'({p.x}, {p.y})')


def calc_r_squared(lista_pontos: ListaPontos2D | ListaPontosND, expressao: sp.Expr) -> float:
    """
    Calcula R² aceitando tanto ListaPontos2D quanto ListaPontosND.
    Caso ListaPontosND, assume que a última coordenada é Y. 
    Para expressão assume que a variavel é x (2D) ou x0, x1, ... (ND).
    """
    pontos = lista_pontos.get_pontos()
    n = lista_pontos.size
    
    # 1. Obter valores reais de Y (Observed)
    if isinstance(lista_pontos, ListaPontosND):
        y_values = lista_pontos.get_index_coordinate_values(-1)
        variaveis_simbolicas = sorted(expressao.free_symbols, key=lambda s: s.name)   
    elif isinstance(lista_pontos, ListaPontos2D):
        y_values = lista_pontos.get_y_values()
        variaveis_simbolicas = [sp.Symbol('x')]
    else:
        raise TypeError("Tipo de lista_pontos não suportado para cálculo de R².")

    y_mean = sum(y_values) / n
    st = sum((y - y_mean) ** 2 for y in y_values)
    
    sr = 0.0 
    for p in pontos:
        if isinstance(lista_pontos, ListaPontosND):
            # mapeia cada símbolo à sua coordenada correspondente
            subs_dict = {}
            for idx, symbol in enumerate(variaveis_simbolicas):
                subs_dict[symbol] = p.get_coordinate(idx)
            
            y_pred = float(expressao.subs(subs_dict).evalf())
            y_real = p.get_coordinate(-1)
            
        else:
            y_pred = float(expressao.subs(variaveis_simbolicas[0], p.x).evalf())
            y_real = p.y

        sr += (y_real - y_pred) ** 2

    if st == 0:
        return 1.0 if sr == 0 else 0.0
        
    r_squared = (st - sr) / st
    return r_squared


def build_expression(ajuste: str, coeficientes: list[float]) -> sp.Expr:
    x = sp.symbols('x')
    if ajuste.lower() in ['reta', 'linear']:
        a0, a1 = coeficientes
        return a0 + a1 * x
    elif ajuste.lower() in ['parabola', 'quadratica']:
        a0, a1, a2 = coeficientes
        return a0 + a1 * x + a2 * x**2
    elif ajuste.lower() in ['exponencial', 'exponential']:
        alfa, beta = coeficientes
        return alfa * sp.exp(beta * x)
    elif ajuste.lower() == 'potencia':
        alfa, beta = coeficientes
        return alfa * x**beta
    elif ajuste.lower() in ['saturacao', 'growth', 'crescimento']:
        alfa, beta = coeficientes
        return (alfa * x) / (beta + x)
    elif ajuste.lower() in ['linear multipla', 'multiple linear']:
        for i, coef in enumerate(coeficientes):
            simb = sp.symbols(f'x{i-1}')
            if i == 0:
                expr = coef
            else:
                expr += coef * simb
        return expr
    else:
        raise ValueError(f"Ajuste '{ajuste}' não reconhecido.")
    