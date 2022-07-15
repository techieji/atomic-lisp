#include "atomic.h"
#include "lexer.h"
#include "parser.h"
#include "objects.h"
#include "stl.h"

int main(void) {
    // char* sample = "(define (abs n) (if (> n 0) n (- n)))";
    // char* sample = "(define a '(Test define test))";
    char* sample = "((lambda (x) (+ x 1)) 1)";
    // char* sample = "(((lambda (x) (lambda (y) (+ x y))) 10) 11)";
    test_runtime_and_stl(sample);
    // repl();
    return 0;
}

void repl(void) {
    char* buf = malloc(100 * sizeof(char));
    struct Env* e = child(NULL);
    load_stl(e);
    e = child(child(e));
    while (true) {
        printf("> ");
        fflush(stdout);
        fgets(buf, 100, stdin);
        struct Object* o = run(parse(&buf), e);
        if (o->type != NIL)
            print_obj(o);
        printf("\n");
    }
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

void test_objects(char* sample) {
    struct ParseTree* pt = parse(&sample);
    print_obj(parsetree_to_obj(pt));
    // print_parsetree(obj_to_parsetree(parsetree_to_obj(pt)));
}

void test_runtime_and_stl(char* sample) {
    struct ParseTree* pt = parse(&sample);
    struct Env* e = child(NULL);
    load_stl(e);
    puts("Loaded stl");
    e = child(e);
    puts("Created top level");
    struct Object* o = run(pt, e);
    print_obj(o);
}