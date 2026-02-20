#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_INTEGER,
    TOKEN_FLOAT,
    TOKEN_STRING,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char *value;
    int line;
    int column;
} Token;

const char* token_type_to_string(TokenType type);
void free_token(Token *token);

#endif // TOKEN_H
