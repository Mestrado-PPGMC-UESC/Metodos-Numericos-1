# taylor_plot.py
import numpy as np
import sympy as sp
import matplotlib.pyplot as plt

# ---------------------------------------
# 1. DEFINA A FUNÇÃO AQUI
# ---------------------------------------
x = sp.symbols('x')

# Exemplo: raiz de (1 + x)
f_sym = sp.sqrt(1 + x)

# ponto de expansão
x0 = 0  # mude se quiser Taylor em torno de outro ponto

# graus dos polinômios de Taylor que você quer comparar
degrees = [1, 3, 5, 7]   # você pode mudar isso para [2,4,6,8], etc.

# intervalo de visualização
x_min, x_max = -0.9, 3   # cuidado: sqrt(1+x) precisa de x > -1
num_points = 400

# ponto onde você quer avaliar aproximação e erros
x_eval = 0.1  # pode trocar por outro (desde que > -1 para sqrt(1+x))
# ---------------------------------------


def taylor_polynomial(f, x_symbol, x0_val, degree):
    """
    Retorna o polinômio de Taylor de ordem 'degree' da função simbólica f(x)
    expandida em torno de x = x0_val.
    """
    series = sp.series(f, x_symbol, x0_val, degree + 1)  # inclui termos até (x-x0)^degree
    poly = sp.expand(series.removeO())
    return poly


def main():
    # cria função numérica da função original
    f_num = sp.lambdify(x, f_sym, 'numpy')

    # vetor de pontos para o gráfico
    xs = np.linspace(x_min, x_max, num_points)
    ys_f = f_num(xs)

    plt.figure(figsize=(10, 6))
    plt.plot(xs, ys_f, label='f(x)', linewidth=2)

    # gera e plota cada polinômio de Taylor
    for n in degrees:
        Tn_sym = taylor_polynomial(f_sym, x, x0, n)
        Tn_num = sp.lambdify(x, Tn_sym, 'numpy')
        ys_Tn = Tn_num(xs)
        plt.plot(xs, ys_Tn, linestyle='--', label=f'Taylor grau {n}')

    # marca o ponto de expansão
    plt.axvline(x=x0, color='k', linewidth=0.8, linestyle=':', label=f'x0 = {x0}')

    plt.title('Aproximação de Taylor')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.show()

    # também imprime os polinômios no console
    print("\nFunção f(x):")
    print(f_sym)
    print(f"\nPonto de expansão x0 = {x0}\n")

    for n in degrees:
        Tn_sym = taylor_polynomial(f_sym, x, x0, n)
        print(f"Polinômio de Taylor de grau {n}:")
        print(f"  T_{n}(x) = {Tn_sym}")

        # ---- Cálculo da aproximação e erros em x_eval ----
        f_exato = float(f_sym.subs(x, x_eval))
        f_aprox = float(Tn_sym.subs(x, x_eval))
        erro_abs = abs(f_exato - f_aprox)
        erro_rel = erro_abs / abs(f_exato) if f_exato != 0 else float('nan')

        print(f"  Avaliando em x = {x_eval}:")
        print(f"    f(x) exato       = {f_exato:.10e}")
        print(f"    T_{n}(x) aprox   = {f_aprox:.10e}")
        print(f"    Erro absoluto    = {erro_abs:.10e}")
        print(f"    Erro relativo    = {erro_rel:.10e}")
        print()  # linha em branco


if __name__ == "__main__":
    main()
