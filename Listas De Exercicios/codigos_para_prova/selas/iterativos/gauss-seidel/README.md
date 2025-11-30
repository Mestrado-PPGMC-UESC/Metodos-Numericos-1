# Método de Gauss–Seidel

Este projeto implementa a resolução de sistemas lineares **Ax = b** usando o **método iterativo de Gauss–Seidel** em linguagem C.

O método de Gauss–Seidel atualiza cada componente de **x** usando imediatamente os valores mais recentes calculados na própria iteração.  
O chute inicial adotado é **x₀ = bᵢ / aᵢᵢ**, e a convergência é verificada pelo **erro relativo** (entre normas consecutivas) usando a **norma infinito**.

---

## 📂 Estrutura do Projeto

- **gaussseidel.c** → Implementação do método de Gauss–Seidel  
- **gaussseidel.h** → Interface (definições e documentação)  
- **utils.c** → Funções auxiliares (leitura, impressão, alocação/liberação de memória)  
- **utils.h** → Interface das utilidades  
- **main.c** → Ponto de entrada: usa Gauss–Seidel para resolver o sistema  
- **entrada.txt** → Arquivo de entrada com o sistema linear  

---

## 📌 Formato do Arquivo de Entrada

O arquivo deve conter:

1. **Primeira linha:** um inteiro `n` (ordem da matriz A).  
2. **Próximas `n` linhas:** cada linha com **`n` coeficientes de A** + **1 coeficiente de b**.  
   - Total por linha: **`n + 1`** valores.  
   - Valores separados por espaço.  
   - Usar **ponto** como separador decimal.

### Exemplo (3×3)

Arquivo `entrada.txt`:

```
3
2  1  -1   8
-3 -1   2 -11
-2  1   2  -3
```

---

## ⚙️ Compilação e Execução

Compilar com **gcc** (ou MinGW no Windows):

```bash
gcc main.c gaussseidel.c utils.c -o gauss_seidel -lm
```

Executar:

```bash
./gauss_seidel
```

---

## 📤 Saída Esperada

1. O programa lê e armazena a matriz estendida `[A|b]`.  
2. Aplica o método de Gauss–Seidel com o chute inicial `bᵢ / aᵢᵢ`.  
3. (Opcional) Mostra o progresso por iteração, se habilitado no código.  
4. Para quando o erro relativo (norma ∞) entre iterações ficar menor que a tolerância.  
5. Mostra o status final e a solução encontrada.

### Exemplo de saída:

```
Gauss-Seidel: convergiu.
Solução do sistema:
x[0] = 2.000000
x[1] = 3.000000
x[2] = -1.000000
```

---

## 🚩 Possíveis Status

- **Gauss-Seidel: convergiu.** → solução encontrada dentro da tolerância.  
- **Gauss-Seidel: não convergiu.** → número máximo de iterações atingido sem convergência.  
- **Gauss-Seidel: sistema singular ou pivô nulo.** → divisão por pivô muito pequeno.  

---

## 💡 Observações

- Convergência mais provável quando **A** é **diagonalmente dominante** ou **simétrica definida positiva (SPD)**.  
- O método costuma convergir **mais rápido** que Jacobi nas mesmas condições.  
