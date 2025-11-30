# Método de Jacobi

Este projeto implementa a resolução de sistemas lineares **Ax = b** usando o **método iterativo de Jacobi** em linguagem C.  

O método de Jacobi é um algoritmo iterativo que atualiza as aproximações da solução a cada iteração.  
O chute inicial adotado é **x₀ = bᵢ / aᵢᵢ**, e a convergência é verificada pelo **erro relativo** usando a **norma infinito**.

---

## 📂 Estrutura do Projeto

- **jacobi.c** → Implementação do método de Jacobi  
- **jacobi.h** → Interface (definições e documentação)  
- **utils.c** → Funções auxiliares (leitura, impressão, alocação/liberação de memória)  
- **utils.h** → Interface das utilidades  
- **main.c** → Ponto de entrada: usa Jacobi para resolver o sistema  
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
gcc main.c jacobi.c utils.c -o jacobi -lm
```

Executar:

```bash
./jacobi
```

---

## 📤 Saída Esperada

1. O programa lê e armazena a matriz estendida `[A|b]`.  
2. Aplica o método de Jacobi com o chute inicial `bᵢ / aᵢᵢ`.  
3. A cada iteração, mostra o vetor solução intermediário.  
4. Para quando o erro relativo na norma infinito for menor que a tolerância.  
5. Mostra o status final e a solução encontrada.  

### Exemplo de saída:

```
Iteração 1: x = [4.000000, 11.000000, -1.500000]
Iteração 2: x = [2.250000, 2.500000, -0.750000]
Iteração 3: x = [2.062500, 2.937500, -1.031250]
...
Jacobi: convergiu.
Solução do sistema:
x[0] = 2.000000
x[1] = 3.000000
x[2] = -1.000000
```

---

## 🚩 Possíveis Status

- **Jacobi: convergiu.** → solução encontrada dentro da tolerância.  
- **Jacobi: não convergiu.** → número máximo de iterações atingido sem convergência.  
- **Jacobi: sistema singular ou diagonal nula.** → divisão por pivô nulo.  
