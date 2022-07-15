#include "stl.h"

struct Object* make_fn(enum FunctionKind kind) {
    struct Object* fn = malloc(sizeof(struct Object));
    fn->type = FUNCTION;
    fn->fn = malloc(sizeof(struct Function));
    fn->fn->kind = kind;
    fn->fn->env = get_current_environment();
    return fn;
}

void load_stl(struct Env* e) {
    struct Object* fn;
    
    EXPOSE(NATIVE, display, "display");
    EXPOSE(SPECIAL, lambda, "lambda");
    EXPOSE(NATIVE, plus, "+");
    EXPOSE(NATIVE, exit_, "exit");
    EXPOSE(SPECIAL, define, "define");
    EXPOSE(NATIVE, display_variables, "display-variables");
}

struct ArgList* obj_to_args(struct Object* bl) {
    struct ArgList* head = malloc(sizeof(struct ArgList)), *al = head;
    while (bl->type != NIL) {
        al->argname = bl->pair->car->s;      // TODO: add error handling
        if (bl->pair->cdr != NULL) {
            al->next = malloc(sizeof(struct ArgList));
            al = al->next;
        } else
            al->next = NULL;
        bl = bl->pair->cdr;
    }
    return head;
}

DECLARE(lambda) {
    struct Object* new_fn = make_fn(NORMAL);
    new_fn->fn->args = obj_to_args(al->obj);
    new_fn->fn->code.code = obj_to_parsetree(al->next->obj);
    return new_fn;
}

DECLARE(define) {
    struct Env* e = get_current_environment()->upper;
    char* varname = al->obj->s;     // TODO: Add error checking
    struct Object* o = run(obj_to_parsetree(al->next->obj), e);
    env_bind(e, new_record(varname, o));
    return o;
}

DECLARE(display) {
    print_obj(al->obj);
    struct Object* o = malloc(sizeof(struct Object));
    o->type = NIL;
    return o;
}

DECLARE(plus) {
    struct Object* o = malloc(sizeof(struct Object));
    o->type = INT;
    o->i = al->obj->i + al->next->obj->i;        // Binary op change in future
    return o;
}

DECLARE(exit_) {
    exit(0);
    return malloc(sizeof(struct Object));
}

DECLARE(display_variables) {
    print_all_variables(get_current_environment());
    struct Object* o = malloc(sizeof(struct Object));
    o->type = NIL;
    return o;
}