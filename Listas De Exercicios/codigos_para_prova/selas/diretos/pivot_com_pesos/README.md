# Método de Eliminação de Gauss com Pivotamento por Pesos (Scaled Partial)

## 📘 Descrição Geral
Este programa implementa o **método da Eliminação de Gauss com Pivotamento Escalonado (por Pesos)**, uma variação mais estável do pivotamento parcial.  
O objetivo é resolver sistemas lineares `Ax = b` com melhor controle numérico, especialmente em matrizes mal-condicionadas como as de **Hilbert**.

A abordagem realiza:
- Seleção escalonada do pivô com base em pesos de escala;
- Troca de linhas ponderada para reduzir erros de arredondamento;
- Substituição regressiva robusta com detecção de instabilidade numérica.

---

## 🧮 Etapas do Método

### 1. **Eliminação com Pivotamento por Pesos**
Cada linha recebe um **peso de escala** `s[i] = max_j |A[i,j]|`.  
A escolha do pivô é feita pela razão:
```
|A[i,k]| / s[i]
```
A linha que maximiza essa razão é usada como pivô, garantindo maior estabilidade.

**Passos principais:**
1. Calcular os pesos de cada linha;  
2. Escolher o pivô escalonado;  
3. Trocar linhas e pesos, se necessário;  
4. Eliminar os elementos abaixo do pivô.

---

### 2. **Substituição Regressiva (Ux = c)**
Após a triangularização, resolve-se o sistema superior `Ux = c`.  
A regressiva utiliza uma **tolerância** para detectar pivôs muito pequenos e ajustar o cálculo da solução, evitando divisões instáveis.

---

### 3. **Status do Resultado**
O algoritmo retorna um dos seguintes status:

| Status | Significado |
|---------|--------------|
| `GAUSS_OK` | Sistema resolvido normalmente |
| `GAUSS_SINGULAR` | Pivôs muito pequenos detectados (instabilidade numérica) |
| `GAUSS_INCONSISTENTE` | Linha nula com termo independente diferente de zero |

Essas condições não interrompem o cálculo — são apenas **diagnósticos** informados ao final.

---

## 🧩 Estrutura dos Arquivos
```
📂 gauss_pivot_com_pesos/
│
├── gauss.c         # Implementação do método (eliminação, regressiva, diagnóstico)
├── gauss.h         # Definições de tipos, constantes e funções públicas
├── utils.c/h       # Funções auxiliares (matriz de Hilbert, erro relativo, impressão)
├── main.c          # Programa principal e medição de desempenho
└── Makefile        # Compilação e execução automatizadas
```

---

## 🚀 Execução

### Compilação
```bash
make
```

### Execução
```bash
make run
```

### Limpeza
```bash
make clean
```

---
