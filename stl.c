#include "stl.h"

struct Object* make_fn(enum FunctionKind kind) {
    struct Object* fn = malloc(sizeof(struct Object));
    fn->type = FUNCTION;
    fn->fn = malloc(sizeof(struct Function));
    fn->fn->kind = kind;
    return fn;
}

void load_stl(struct Env* e) {
    struct Object* fn = make_fn(NATIVE);
    fn->fn->code.native_fn = display;
    env_bind(e, new_record("display", fn));
}

DECLARE(display) {
    print_obj(al->obj);
    struct Object* o = malloc(sizeof(struct Object));
    o->type = NIL;
    return o;
}