#include "lexer.h"
// #include "helpers.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    const char *text;
    Token_Kind kind;
} Literal_Token;

Literal_Token literal_tokens[] = {
    {.text = "(", .kind = TOKEN_OPEN_PAREN},
    {.text = ")", .kind = TOKEN_CLOSE_PAREN},
    {.text = "{", .kind = TOKEN_OPEN_CURLY},
    {.text = "}", .kind = TOKEN_CLOSE_CURLY},
    {.text = ";", .kind = TOKEN_SEMICOLON},
};

#define literal_tokens_count                                                   \
  (sizeof(literal_tokens) / sizeof(literal_tokens[0]))

const char *token_kind_name(Token_Kind kind) {
    switch (kind) {
    case TOKEN_END:
        return "end of content";
    case TOKEN_INVALID:
        return "invalid token";
    case TOKEN_PREPROC:
        return "preprocessor directive";
    case TOKEN_SYMBOL:
        return "symbol";
    case TOKEN_OPEN_PAREN:
        return "open paren";
    case TOKEN_CLOSE_PAREN:
        return "close paren";
    case TOKEN_OPEN_CURLY:
        return "open curly";
    case TOKEN_CLOSE_CURLY:
        return "close curly";
    case TOKEN_SEMICOLON:
        return "semicolon";
    case TOKEN_NUMBER:
        return "number";
    default:
        return "unknown token";
    };
    return NULL; // fallback
};

Lexer lexer_new(const char *content, size_t content_len) {
    Lexer l = {0};
    l.content = content;
    l.content_len = content_len;
    return l;
}

bool lexer_starts_with(Lexer *l, const char *prefix) {
    size_t prefix_len = strlen(prefix);
    if (prefix_len == 0) {
        return true;
    }
    if (l->cursor + prefix_len - 1 >= l->content_len) {
        return false;
    }
    for (size_t i = 0; i < prefix_len; ++i) {
        if (prefix[i] != l->content[l->cursor + i]) {
            return false;
        }
    }
    return true;
}

char lexer_chop_char(Lexer *l) {
    assert(l->cursor < l->content_len);
    char x = l->content[l->cursor];
    l->cursor += 1;
    if (x == '\n') {
        l->line += 1;
        l->bol = l->cursor;
    }
    return x;
}

void lexer_trim_left(Lexer *l) {
    while (l->cursor < l->content_len && isspace(l->content[l->cursor])) {
        lexer_chop_char(
            l); // it returns a value but I don't need it for now so I won't save it
    }
}

bool is_symbol_start(char x) {
    return isalpha(x) || x == '_';
}

bool is_symbol(char x) {
    return isalnum(x) || x == '_';
}

Token lexer_next(Lexer *l) {
    lexer_trim_left(l); // remove white spaces

    // start prev
    // Token token = {.text = &l->content[l->cursor]};

    // if (l->cursor >= l->content_len)
    //     return token;
    //  end prev

    Token token = {.text = &l->content[l->cursor], .kind = TOKEN_END};

    if (l->cursor >= l->content_len) {
        token.kind = TOKEN_END;
        return token;
    }

    if (l->content[l->cursor] == '#') {
        token.kind = TOKEN_PREPROC;
        while (l->cursor < l->content_len && l->content[l->cursor] != '\n') {
            // since preprocessor directives are valid until end of line, if != \n,
            // continue tokenizing
            token.text_len += 1;
            lexer_chop_char(l);
        }
        if (l->cursor < l->content_len) {
            lexer_chop_char(l);
        }
        return token;
    }

    for (size_t i = 0; i < literal_tokens_count; ++i) {
        if (lexer_starts_with(l, literal_tokens[i].text)) {
            // NOTE: assuming that no newlines are in literal_tokens[i].text, which is
            // not actually valid on cpp
            size_t text_len = strlen(literal_tokens[i].text);
            token.kind = literal_tokens[i].kind;
            token.text_len = text_len;
            l->cursor += text_len;
            return token;
        }
    }

    // new
    if (isdigit(l->content[l->cursor])) {
        token.kind = TOKEN_NUMBER;
        while (l->cursor < l->content_len && isdigit(l->content[l->cursor])) {
            token.text_len += 1;
            lexer_chop_char(l);
        }
        if (l->cursor < l->content_len && l->content[l->cursor] == '.') {
            // handle decimal numbers
            token.text_len += 1;
            lexer_chop_char(l);
            while (l->cursor < l->content_len && isdigit(l->content[l->cursor])) {
                token.text_len += 1;
                lexer_chop_char(l);
            }
        }
        return token;
    }
    // new

    if (is_symbol_start(l->content[l->cursor])) {
        token.kind = TOKEN_SYMBOL;
        while (l->cursor < l->content_len && is_symbol(l->content[l->cursor])) {
            l->cursor += 1;
            token.text_len += 1;
        }
        return token;
    }

    return token;
}
