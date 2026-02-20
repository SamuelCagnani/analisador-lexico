#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "token.h"

char* read_file(const char* path) {
    FILE* file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "Could not open file \"%s\".\n", path);
        exit(74);
    }

    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(fileSize + 1);
    if (!buffer) {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
        exit(74);
    }

    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if (bytesRead < fileSize) {
        fprintf(stderr, "Could not read file \"%s\".\n", path);
        exit(74);
    }
    buffer[bytesRead] = '\0';

    fclose(file);
    return buffer;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: lexer <path>\n");
        return 64;
    }

    char* source = read_file(argv[1]);

    Lexer lexer;
    init_lexer(&lexer, source);

    Token token;
    do {
        token = next_token(&lexer);

        // Print token: <TYPE, "value", line, col>
        printf("<%s, \"%s\", %d, %d>\n",
               token_type_to_string(token.type),
               token.value,
               token.line,
               token.column);

        free_token(&token);
    } while (token.type != TOKEN_EOF);

    free(source);
    return 0;
}
