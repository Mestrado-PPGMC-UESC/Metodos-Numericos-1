# 🧮 Eliminação de Gauss — Pivotamento Total (com Tolerância)

Este módulo implementa o **método da Eliminação de Gauss com pivotamento total**, incluindo suporte a **tolerância numérica** e **aviso de singularidade**.  
A matriz de Hilbert é usada como sistema de teste, permitindo avaliar o comportamento em casos mal-condicionados.

---

## 📂 Estrutura dos arquivos

```
├── gauss.c
├── gauss.h
├── utils.c
├── utils.h
├── main.c
├── Makefile
```

---

## ⚙️ Descrição geral

O **pivotamento total** procura o maior elemento em valor absoluto **em toda a submatriz restante**, trocando tanto linhas quanto colunas logicamente.  
A implementação considera uma **tolerância** para lidar com pivôs muito pequenos, evitando divisões numéricas instáveis.

Durante a execução:
- **Pivôs próximos de zero** acionam a flag `gaussFlagPivoQuaseZero()`;
- **Sistemas singulares** e **inconsistentes** são detectados, mas o programa **não é interrompido**;
- A decisão final (OK / SINGULAR / INCONSISTENTE) é feita no retorno de `gauss()`.

---

## 🧠 Funções principais

| Função | Descrição |
|--------|------------|
| `eliminacao_total()` | Executa a fase de eliminação com pivotamento total e tolerância. |
| `substituicaoRegressiva_total()` | Resolve o sistema triangular superior resultante. |
| `gauss()` | Wrapper completo: executa as duas fases e retorna o status final. |
| `imprimirStatus()` | Exibe uma mensagem textual sobre o status do sistema. |

---

## 🔍 Status de retorno

| Código | Significado |
|--------|-------------|
| `GAUSS_OK` | Sistema resolvido com sucesso. |
| `GAUSS_SINGULAR` | Sistema singular ou numericamente instável (pivô ~ 0). |
| `GAUSS_INCONSISTENTE` | Sistema inconsistente (linha nula com b ≠ 0). |

---

## ⚡ Execução

Compile com:

```bash
make
```

Execute com:

```bash
make run
```

