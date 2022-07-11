#include "atomic.h"
#include "lexer.h"
#include "parser.h"
#include "objects.h"
#include "stl.h"

int main(void) {
    // char* sample = "(define (abs n) (if (> n 0) n (- n)))";
    // char* sample = "(define a '(Test define test))";
    char* sample = "(display \"Testing\n\")";
    test_runtime_and_stl(sample);
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

void test_objects(char* sample) {
    struct ParseTree* pt = parse(&sample);
    print_obj(parsetree_to_obj(pt));
    // print_parsetree(obj_to_parsetree(parsetree_to_obj(pt)));
}

void test_runtime_and_stl(char* sample) {
    struct ParseTree* pt = parse(&sample);
    struct Env* e = child(NULL);
    load_stl(e);
    e = child(e);
    struct Object* o = run(pt, e);
    print_obj(o);
}