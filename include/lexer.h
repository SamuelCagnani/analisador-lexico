#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef struct {
    const char *source;
    int position;
    int line;
    int column;
} Lexer;

void init_lexer(Lexer *lexer, const char *source);
Token next_token(Lexer *lexer);

#endif // LEXER_H
