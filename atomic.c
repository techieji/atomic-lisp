#include "atomic.h"
#include "lexer.h"
#include "parser.h"

int main(void) {
    // char* sample = "(define (abs n) (if (> n 0) n (- n)))";
    char* sample = "(define a #t)";
    test_lexer(sample);
    return 0;
}

void test_lexer(char* sample) {
    struct Token* t;
    while ((t = get_next_token(&sample))->type != TOKEN_EOF)
        print_token(t);
}

void test_parser(char* sample) {
    struct ParseTree* pt = parse(&sample);
    print_parsetree(pt);
}