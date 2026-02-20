#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include "token.h"

// Helper: Check if a string is a keyword
static int is_keyword(const char *word) {
    const char *keywords[] = {
        "programa", "inicio", "fim", "inteiro", "flutuante", "se", "entao", "senao", "fimse",
        "para", "de", "ate", "passo", "faca", "fimpara", "enquanto", "fimenquanto", "e", "ou",
        "nao", "leia", "escreva", "escreval", "procedimento", "retorna", "vazio", "div", NULL
    };
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(word, keywords[i]) == 0) return 1;
    }
    return 0;
}

void init_lexer(Lexer *lexer, const char *source) {
    lexer->source = source;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
}

static char peek(Lexer *lexer) {
    return lexer->source[lexer->position];
}

static char peek_next(Lexer *lexer) {
    if (lexer->source[lexer->position] == '\0') return '\0';
    return lexer->source[lexer->position + 1];
}

static char advance(Lexer *lexer) {
    char c = lexer->source[lexer->position];
    if (c == '\0') return '\0';
    lexer->position++;
    if (c == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    return c;
}

static void skip_whitespace(Lexer *lexer) {
    while (1) {
        char c = peek(lexer);
        if (c == ' ' || c == '\t' || c == '\r') {
            advance(lexer);
        } else if (c == '\n') {
            advance(lexer);
        } else if (c == '/' && peek_next(lexer) == '/') {
            // Single line comment
            while (peek(lexer) != '\n' && peek(lexer) != '\0') {
                advance(lexer);
            }
        } else if (c == '/' && peek_next(lexer) == '*') {
            // Multi-line comment
            advance(lexer); // consume /
            advance(lexer); // consume *
            while (1) {
                if (peek(lexer) == '\0') {
                     fprintf(stderr, "Error: Unterminated block comment at line %d, column %d\n", lexer->line, lexer->column);
                     exit(1);
                }
                if (peek(lexer) == '*' && peek_next(lexer) == '/') {
                    advance(lexer); // consume *
                    advance(lexer); // consume /
                    break;
                }
                advance(lexer);
            }
        } else {
            break;
        }
    }
}

// Adjusted make_token to accept start_column
static Token make_token_col(TokenType type, const char *start, int length, int line, int column) {
    Token token;
    token.type = type;
    token.value = (char *)malloc(length + 1);
    if (!token.value) {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }
    strncpy(token.value, start, length);
    token.value[length] = '\0';
    token.line = line;
    token.column = column;
    return token;
}

Token next_token(Lexer *lexer) {
    skip_whitespace(lexer);

    char c = peek(lexer);
    if (c == '\0') {
        return make_token_col(TOKEN_EOF, "EOF", 3, lexer->line, lexer->column);
    }

    int start_line = lexer->line;
    int start_col = lexer->column;
    const char *start_ptr = &lexer->source[lexer->position];

    if (isalpha(c) || c == '_') {
        // Identifier or Keyword
        while (isalnum(peek(lexer)) || peek(lexer) == '_') {
            advance(lexer);
        }
        int length = (int)(&lexer->source[lexer->position] - start_ptr);
        char *word = (char *)malloc(length + 1);
        strncpy(word, start_ptr, length);
        word[length] = '\0';

        TokenType type = is_keyword(word) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
        free(word);
        return make_token_col(type, start_ptr, length, start_line, start_col);
    }

    if (isdigit(c)) {
        // Number
        TokenType type = TOKEN_INTEGER;
        while (isdigit(peek(lexer))) {
            advance(lexer);
        }
        // Float part
        if (peek(lexer) == '.' && isdigit(peek_next(lexer))) {
            type = TOKEN_FLOAT;
            advance(lexer); // consume .
            while (isdigit(peek(lexer))) {
                advance(lexer);
            }
        }
        // Exponent part
        if (peek(lexer) == 'e' || peek(lexer) == 'E') {
            type = TOKEN_FLOAT;
            char next = peek_next(lexer);
            if (next == '+' || next == '-' || isdigit(next)) {
                 advance(lexer); // consume e/E
                 if (peek(lexer) == '+' || peek(lexer) == '-') {
                     advance(lexer);
                 }
                 while (isdigit(peek(lexer))) {
                     advance(lexer);
                 }
            }
        }
        int length = (int)(&lexer->source[lexer->position] - start_ptr);
        return make_token_col(type, start_ptr, length, start_line, start_col);
    }

    if (c == '"') {
        // String
        advance(lexer); // consume opening quote
        // start_ptr needs to point after the quote for the value, or include quotes?
        // Usually token value for string is the content. But let's keep quotes to be safe or remove them?
        // PRD: value (String): The literal text extracted from the source code.
        // Usually implies with quotes or without?
        // Example: "x", "42", "==". "42" is the text.
        // For string "hello", the literal text in source code is "hello" (with quotes).
        // Let's include quotes in the value to match the raw source extraction principle.

        while (peek(lexer) != '"' && peek(lexer) != '\0') {
            if (peek(lexer) == '\n') {
                fprintf(stderr, "Error: Unclosed string at line %d, column %d\n", start_line, start_col);
                exit(1);
            }
            if (peek(lexer) == '\\') {
                advance(lexer); // consume backslash
                // handle escape logic if needed, but for now just consume next char
                if (peek(lexer) != '\0') advance(lexer);
            } else {
                advance(lexer);
            }
        }

        if (peek(lexer) == '\0') {
             fprintf(stderr, "Error: Unclosed string at EOF, started at line %d\n", start_line);
             exit(1);
        }

        advance(lexer); // consume closing quote
        int length = (int)(&lexer->source[lexer->position] - start_ptr);
        // Remove quotes for the value?
        // If I keep quotes, use start_ptr and length.
        // PRD says: value (String): The literal text extracted from the source code.
        // If I have `x = "hello"`, token value for string is `"hello"`.
        return make_token_col(TOKEN_STRING, start_ptr, length, start_line, start_col);
    }

    // Operators and Delimiters
    advance(lexer); // consume the character

    // Check for 2-char operators
    // ==, !=, <=, >=, &&, ||
    // start_ptr points to the first char.
    // c is the first char.

    if ((c == '=' && peek(lexer) == '=') ||
        (c == '!' && peek(lexer) == '=') ||
        (c == '<' && peek(lexer) == '=') ||
        (c == '>' && peek(lexer) == '=') ||
        (c == '&' && peek(lexer) == '&') ||
        (c == '|' && peek(lexer) == '|')) {
            advance(lexer);
            return make_token_col(TOKEN_OPERATOR, start_ptr, 2, start_line, start_col);
    }

    // Single char operators
    // +, -, *, /, %, =, <, >, !, & (bitwise?), | (bitwise?)
    // PRD: =, !, <, >, &, |, +, -, *, /
    if (strchr("=!<>&|+-*/", c)) {
         return make_token_col(TOKEN_OPERATOR, start_ptr, 1, start_line, start_col);
    }

    // Delimiters
    // ;, ,, (, ), {, }
    if (strchr(";,(){},", c)) {
        return make_token_col(TOKEN_DELIMITER, start_ptr, 1, start_line, start_col);
    }

    fprintf(stderr, "Error: Unexpected character '%c' at line %d, column %d\n", c, start_line, start_col);
    exit(1);
}
