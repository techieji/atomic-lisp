#include "objects.h"

struct Object* parsetree_to_obj(struct ParseTree* pt) {
    struct Object* o = malloc(sizeof(struct Object));
    switch (pt->type) {
        case LEAF_INT:
            o->type = INT;
            o->i = pt->i;
            return o;
        case LEAF_DOUBLE:
            o->type = DOUBLE;
            o->d = pt->d;
            return o;
        case LEAF_BOOL:
            o->type = BOOL;
            o->b = pt->b;
            return o;
        case LEAF_STRING:
            o->type = STRING;
            o->s = pt->s;
            return o;
        case LEAF_SYMBOL:
            o->type = SYMBOL;
            o->s = pt->s;
            return o;
        case TREE:
            return branchlist_to_obj(pt->branches);
    }
    return NULL;  // To satisfy type-checker
}

struct Object* branchlist_to_obj(struct BranchList* bl) {
    struct Object* o = malloc(sizeof(struct Object));
    if (bl == NULL)
        o->type = NIL;
    else {
        o->type = PAIR;
        o->pair = malloc(sizeof(struct Pair));
        o->pair->car = parsetree_to_obj(bl->tree);
        o->pair->cdr = branchlist_to_obj(bl->next);      // Recursive procedure
    }
    return o;
}

struct ParseTree* obj_to_parsetree(struct Object* o) {
    struct ParseTree* pt = malloc(sizeof(struct ParseTree));
    switch (o->type) {       // Yes, I know the switch statement can be simplified by simply "casting" between enum types. But I'm aiming for clarity here.
        case INT:
            pt->type = LEAF_INT;
            pt->i = o->i;
            return pt;
        case DOUBLE:
            pt->type = LEAF_DOUBLE;
            pt->d = o->d;
            return pt;
        case BOOL:
            pt->type = LEAF_BOOL;
            pt->b = o->b;
            return pt;
        case STRING:
            pt->type = LEAF_STRING;
            pt->s = o->s;
            return pt;
        case SYMBOL:
            pt->type = LEAF_SYMBOL;
            pt->s = o->s;
            return pt;
        case PAIR:
            pt->type = TREE;
            pt->branches = malloc(sizeof(struct BranchList));
            pt->branches->tree = obj_to_parsetree(o->pair->car);
            pt->branches->next = obj_to_parsetree(o->pair->cdr)->branches;    // TODO: Add error handling
            return pt;
        case NIL:
            pt->type = TREE;
            pt->branches = NULL;
            return pt;
        case FUNCTION:
            puts("FUNCTION??");     // TODO: Add error handling
            exit(1);
    }
    return NULL;   // To satisfy type-checker
}

static int quoted = 0;
void print_obj(struct Object* o) {
    switch (o->type) {
        case INT:
            printf("%d", o->i);
            return;
        case DOUBLE:
            printf("%g", o->d);
            return;
        case BOOL:
            printf("%s", o->b ? "#t" : "#f");
            return;
        case STRING:
            printf("\"%s\"", o->s);
            return;
        case SYMBOL:
            if (!quoted) printf("'");
            printf("%s", o->s);
            return;
        case PAIR:
            if (!quoted++) printf("'");
            printf("(");
            while (o->type == PAIR) {
                print_obj(o->pair->car);
                printf(" ");
                o = o->pair->cdr; 
            }
            if (o->type == NIL)
                printf("\b)");
            else {
                printf(". ");
                print_obj(o);
                printf(")");
            }
            quoted--;
            return;
        case NIL:
            if (!quoted) printf("'");
            printf("()");
            return;
        case FUNCTION:
            printf("<procedure>");     // Maybe print out define/lambda syntax of function? Consistent with the return value evaluating to itself...
            return;
    }
}

struct Record* new_record(char* varname, struct Object* obj) {
    struct Record* ret = malloc(sizeof(struct Record));
    ret->varname = varname;
    ret->obj = obj;
    return ret;
}

void env_bind(struct Env* e, struct Record* record) {
    printf("Varname being bound: %s\n", record->varname);
    if (e->scope == NULL) {
        e->scope = malloc(sizeof(struct Scope));
        e->scope->rec = record;
        e->scope->next = NULL;
    } else {
        struct Scope* s = e->scope;
        while (s->next != NULL) s = s->next;
        s->next = malloc(sizeof(struct Scope));
        s->next->rec = record;
        s->next->next = NULL;
    }
}

struct Object* env_get(struct Env* e, char* varname) {
    while (e != NULL) {
        struct Scope* s = e->scope;
        while (s != NULL) {
            //printf("On var %s\n", s->rec->varname);
            if (strcmp(s->rec->varname, varname) == 0)
                return s->rec->obj;
            else
                s = s->next;
        }
        e = e->upper;
    }
    printf("%s not found!!\n", varname);
    return NULL;      // TODO: Add error handling
}

static struct Env* _current_env;

struct Env* child(struct Env* parent) {
    struct Env* ret = malloc(sizeof(struct Env));
    ret->scope = NULL;
    ret->upper = parent;
    _current_env = ret;
    if (parent == NULL) puts("Creating top level...");
    puts("NEW ENV");
    print_scope(_current_env->scope);
    return ret;
}

void pop_env(void) {
    _current_env = _current_env->upper;
    puts("POP ENV");
}

struct Env* get_current_environment(void) {
    return _current_env;
}

void print_all_variables(struct Env* e) {
    puts("ENV INFO");
    puts("=========");
    int i;
    struct Env* p;
    for (i = 0, p = e; p != NULL; i++, p = p->upper);
    printf("Layers: %d\n", i);
    i = 1;
    while (e != NULL) {
        printf("SCOPE %i:\n", i);
        print_scope(e->scope);
        e = e->upper;
        i++;
    }
}

void print_scope(struct Scope* s) {
    while (s != NULL) {
        printf("Variable: %s\n", s->rec->varname);
        s = s->next;
    }
}