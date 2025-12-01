# lagrange_master.py
import numpy as np
import sympy as sp
import matplotlib.pyplot as plt

# ===========================================
# CONFIGURAÇÕES GERAIS
# ===========================================
x = sp.symbols('x')

# --- Defina aqui a função f(x) para os modos que usam função ---
# Exemplo: f(x) = 1/x
f_sym = 1/x
# Outros exemplos:
# f_sym = sp.sqrt(1 + x)
# f_sym = sp.exp(x)
# f_sym = sp.sin(x)

# MODO DE USO:
# 1 = pontos equidistantes + função
# 2 = pontos arbitrários + função
# 3 = pontos com coordenadas (x,y) dados
# 4 = pontos de Chebyshev + função
modo = 2

# Intervalo para os modos 1 e 4 (e 1/2 se quiser ver o gráfico)
a, b = 1, 4        # atenção ao domínio (para 1/x, não pode passar por 0)
n_nodes = 3        # número de pontos (nós) de interpolação

# Pontos arbitrários para modo 2 (x) – y virá de f(x)
x_modo2 = np.array([1.0, 2.0, 4.0])

# Pontos (x,y) para modo 3 (coordenadas puras)
x_modo3 = np.array([2.0, 2.5, 4.0])
y_modo3 = np.array([0.5, 0.4, 0.25])
# ===========================================


def gerar_pontos(modo, f_sym):
    """
    Gera x_data, y_data e uma flag indicando se temos função exata (para calcular erro).
    """
    f_num = sp.lambdify(x, f_sym, 'numpy')

    if modo == 1:
        # Pontos equidistantes no intervalo [a, b], y = f(x)
        x_data = np.linspace(a, b, n_nodes)
        y_data = f_num(x_data)
        tem_funcao = True

    elif modo == 2:
        # Pontos arbitrários em x, y = f(x)
        x_data = np.array(x_modo2, dtype=float)
        y_data = f_num(x_data)
        tem_funcao = True

    elif modo == 3:
        # Pontos (x,y) dados diretamente (sem função)
        x_data = np.array(x_modo3, dtype=float)
        y_data = np.array(y_modo3, dtype=float)
        tem_funcao = False

    elif modo == 4:
        # Pontos de Chebyshev no intervalo [a,b], y = f(x)
        # Fórmula: x_i = (a+b)/2 + (b-a)/2 * cos((2i+1)/(2n) * pi)
        i = np.arange(n_nodes)
        x_cheb = (a + b)/2 + (b - a)/2 * np.cos((2*i + 1) * np.pi / (2*n_nodes))
        x_data = np.array(x_cheb, dtype=float)
        y_data = f_num(x_data)
        tem_funcao = True

    else:
        raise ValueError("Modo inválido. Use 1, 2, 3 ou 4.")

    return x_data, y_data, tem_funcao


def lagrange_poly(x_vals, y_vals):
    """
    Retorna:
      (1) P(x) numérica (função Python)
      (2) P(x) simbólica (expressão SymPy não expandida)
      (3) P(x) simbólica expandida (polinômio tradicional)
    """
    x_sym = sp.symbols('x')
    n = len(x_vals)
    poly = 0

    for k in range(n):
        Lk = 1
        for j in range(n):
            if j != k:
                Lk *= (x_sym - x_vals[j]) / (x_vals[k] - x_vals[j])
        poly += y_vals[k] * Lk

    poly_expanded = sp.expand(poly)
    P_num = sp.lambdify(x_sym, poly_expanded, 'numpy')

    return P_num, poly, poly_expanded


def main():
    print(f"\n=== MODO {modo} SELECIONADO ===")
    if   modo == 1: print("Pontos equidistantes + função")
    elif modo == 2: print("Pontos arbitrários + função")
    elif modo == 3: print("Pontos coordenadas (x,y) dados")
    elif modo == 4: print("Pontos de Chebyshev + função")

    # Gera os pontos de acordo com o modo
    x_data, y_data, tem_funcao = gerar_pontos(modo, f_sym)

    # Constrói o polinômio de Lagrange
    P, poly_raw, poly_expanded = lagrange_poly(x_data, y_data)

    # Intervalo para gráfico
    x_min = min(x_data) - 0.5
    x_max = max(x_data) + 0.5
    xs = np.linspace(x_min, x_max, 500)
    ys_P = P(xs)

    # ==================== GRÁFICO ====================
    plt.figure(figsize=(10, 5))

    if tem_funcao:
        f_num = sp.lambdify(x, f_sym, 'numpy')
        ys_f = f_num(xs)
        plt.plot(xs, ys_f, label='f(x)', linewidth=2)

    plt.plot(xs, ys_P, '--', label='Polinômio de Lagrange', linewidth=2)
    plt.scatter(x_data, y_data, color='red', zorder=5, label='Pontos de interpolação')

    plt.title('Interpolação de Lagrange')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.show()

    # ==================== POLINÔMIO ====================
    print("\n==================== POLINÔMIO DE LAGRANGE ====================")
    print("\nForma não expandida (soma dos L_k):\n")
    print(poly_raw)

    print("\nForma expandida (polinômio tradicional):\n")
    print(poly_expanded)

    # ==================== ERROS (SE TIVER FUNÇÃO) ====================
    if tem_funcao:
        print("\n==================== ERRO NO INTERVALO ====================")
        f_num = sp.lambdify(x, f_sym, 'numpy')
        ys_f = f_num(xs)
        erro_abs = np.abs(ys_f - ys_P)
        erro_max = np.max(erro_abs)
        print(f"Erro absoluto máximo no intervalo [{x_min:.3f}, {x_max:.3f}]: {erro_max:.6e}")

        # Também mostra erro em alguns pontos de teste
        pontos_teste = np.linspace(x_min, x_max, 4)
        print("\nErros em alguns pontos de teste:")
        for x0 in pontos_teste:
            real = float(f_num(x0))
            aprox = float(P(x0))
            ea = abs(real - aprox)
            er = ea / abs(real) if real != 0 else float('nan')
            print(f"x = {x0:.4f}  f(x)={real:.6e}  P(x)={aprox:.6e}  "
                  f"Erro abs={ea:.3e}  Erro rel={er:.3e}")
    else:
        print("\n(Não há função exata associada neste modo, então não há cálculo de erro.)")


if __name__ == "__main__":
    main()
