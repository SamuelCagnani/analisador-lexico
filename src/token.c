#include <stdlib.h>
#include "token.h"

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_KEYWORD: return "KEYWORD";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_INTEGER: return "INTEGER";
        case TOKEN_FLOAT: return "FLOAT";
        case TOKEN_STRING: return "STRING";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_DELIMITER: return "DELIMITER";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

void free_token(Token *token) {
    if (token->value) {
        free(token->value);
        token->value = NULL;
    }
}
