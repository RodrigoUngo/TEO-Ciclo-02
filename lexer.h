// #include "helpers.h"
#include <stdlib.h>

#ifndef LEXER_H_
#define LEXER_H_

typedef enum {
    TOKEN_END = 0, // returned token at the end of the string (null char)
    TOKEN_INVALID,
    TOKEN_PREPROC, // preprocessor directives
    TOKEN_SYMBOL,
    TOKEN_OPEN_PAREN,
    TOKEN_CLOSE_PAREN,
    TOKEN_OPEN_CURLY,
    TOKEN_CLOSE_CURLY,
    TOKEN_SEMICOLON,
    TOKEN_NUMBER,
} Token_Kind;

typedef struct {
    const char *file_path;
    size_t row;
    size_t col;
} Loc;

typedef struct {
    Token_Kind kind;
    const char *text;
    size_t text_len;
} Token;

typedef struct {
    const char *content;
    size_t content_len;
    size_t cursor; // absolute position
    size_t line;   // line
    size_t bol;    // beginning of the line
} Lexer;

// Lexer lexer_new(const char *content, size_t content_len);
// Token lexer_next(Lexer *l);

#endif // LEXER_H_
