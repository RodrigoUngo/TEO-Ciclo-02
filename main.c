#include "./lexer.c"
#include "./lexer.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    const char *content = "#include <string.h>\n"
                          "#include <stdio.h>\n"
                          "\n"
                          "int main(void){\n"
                          "    return 0; \n"
                          "}\n";
    Lexer l = lexer_new(content, strlen(content));
    Token t;
    t = lexer_next(&l);
    while (t.kind != TOKEN_END) {
        fprintf(stderr, "%.*s (%s)\n", (int)t.text_len, t.text,
                token_kind_name(t.kind));
        t = lexer_next(&l);
    }
    return 0;
}
