# Método de Gauss–Seidel com Sobre-Relaxamento (SOR)

Este projeto implementa a resolução de sistemas lineares **Ax = b** usando **Gauss–Seidel com Sobre-Relaxamento (SOR)** em linguagem C.

O SOR introduz um fator **ω (omega)** para acelerar (ou estabilizar) a convergência:
\[
x_i^{(k+1)} = (1-\omega)\,x_i^{(k)} \;+\; \omega \cdot x_{i,\text{GS}}^{(k+1)}
\]
com `0 < ω < 2` e `ω = 1` recuperando o Gauss–Seidel clássico.  
O chute inicial é **x₀ = bᵢ / aᵢᵢ**, e a convergência é verificada pelo **erro relativo** entre normas infinito consecutivas.

---

## 📂 Estrutura do Projeto

- **gaussseidel_sor.c** → Implementação do método SOR  
- **gaussseidel_sor.h** → Interface (definições e documentação)  
- **utils.c** → Funções auxiliares  
- **utils.h** → Interface das utilidades  
- **main.c** → Ponto de entrada: usa SOR para resolver o sistema  
- **entrada.txt** → Arquivo de entrada com o sistema linear  

---

## 📌 Formato do Arquivo de Entrada

Mesmo formato:

```
3
2  1  -1   8
-3 -1   2 -11
-2  1   2  -3
```

---

## ⚙️ Compilação e Execução

```bash
gcc main.c gaussseidel_sor.c utils.c -o sor -lm
```

Execução:

```bash
./sor
```

---

## 📤 Saída Esperada

1. O programa lê `[A|b]`.  
2. Executa SOR com **ω** informado no `main.c`.  
3. (Opcional) Mostra o progresso por iteração, se habilitado no código.  
4. Para quando o erro relativo (norma ∞) entre iterações ficar menor que a tolerância.  
5. Mostra o status final e a solução.

### Exemplo de saída:

```
SOR: convergiu.
Solução do sistema:
x[0] = 2.000000
x[1] = 3.000000
x[2] = -1.000000
```

---

## 🚩 Possíveis Status

- **SOR: convergiu.** → solução encontrada dentro da tolerância.  
- **SOR: não convergiu.** → número máximo de iterações atingido sem convergência.  
- **SOR: sistema singular ou pivô nulo.** → divisão por pivô muito pequeno.  
- **SOR: parâmetro omega inválido.** → é obrigatório `0 < ω < 2`.

---

## 🎯 Escolha de ω (omega)

- `ω = 1` → Gauss–Seidel padrão.  
- `1 < ω < 2` → *over-relaxation* (pode acelerar bastante, mas cuidado com instabilidade).  
- `0 < ω < 1` → *under-relaxation* (mais estável, porém mais lento).  
- Em matrizes **SPD** ou **diagonalmente dominantes**, ω entre **1.0 e 1.5** costuma ser um bom ponto de partida.

---

## 💡 Observações

- Tal como GS/Jacobi, a convergência depende das propriedades de **A**; **dominância diagonal** ou **SPD** ajudam.  
- Você pode habilitar *prints* por iteração no código para acompanhar `||x||_inf` e o **erro relativo**.  
