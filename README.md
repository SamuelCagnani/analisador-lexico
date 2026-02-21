# 🔎 Analisador Léxico

Implementação de um **Analisador Léxico em C**, desenvolvido como parte dos estudos na disciplina de Compiladores.

A análise léxica é a primeira etapa do processo de compilação. Ela é responsável por ler o código-fonte caractere por caractere e agrupá-los em unidades chamadas **tokens**, que serão posteriormente utilizadas pela análise sintática.

---

## 📌 Objetivo

Este projeto tem como objetivo:

- Implementar um analisador léxico funcional em C
- Reconhecer e classificar tokens
- Identificar erros léxicos
- Servir como base para futuras evoluções (como um analisador sintático)

---

## 🚀 Funcionalidades

O analisador é capaz de:

- Ler arquivos de entrada
- Identificar:
  - Identificadores
  - Números
  - Operadores
  - Delimitadores
  - Palavras-chave (se implementadas)
- Ignorar espaços em branco
- Detectar erros léxicos
- Exibir os tokens reconhecidos

---

## 📁 Estrutura do Projeto

```
.
├── include/              # Arquivos de cabeçalho (.h)
├── src/                  # Código-fonte em C
├── test.simples          # Arquivo de teste válido
├── error_test.simples    # Arquivo com erro léxico
├── test_lexer.sh         # Script para testes
├── Makefile              # Automação de build
└── README.md
```

---

## 🛠️ Requisitos

- GCC (ou outro compilador C)
- Make

---

## 🔨 Compilação

Para compilar o projeto, execute:

```bash
make
```

Isso irá gerar o executável do analisador.

---

## ▶️ Execução

Para executar o analisador:

```bash
./analisador < arquivo.simples
```

Ou para rodar os testes automatizados:

```bash
sh test_lexer.sh
```

---

## 🧪 Exemplos

### Arquivo válido

```c
int x = 10;
```

### Arquivo com erro

```c
int @x = 10;
```

---

## 📚 Conceito

A análise léxica transforma o código-fonte em uma sequência estruturada de tokens, como:

```
[TIPO_INT] [IDENTIFICADOR:x] [OPERADOR:=] [NUMERO:10] [DELIMITADOR:;]
```

Esses tokens são então utilizados pela próxima fase do compilador.

---

## 📌 Próximos Passos

- Implementar analisador sintático
- Melhorar tratamento de erros
- Adicionar testes automatizados mais robustos
- Expandir suporte a mais tokens

---

## 👨‍💻 Autor

Samuel Cagnani  
Engenharia de Computação  

---

## 📄 Licença

Projeto desenvolvido para fins educacionais.