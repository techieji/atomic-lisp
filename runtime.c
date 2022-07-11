#include "runtime.h"

struct Object* run(struct ParseTree* pt, struct Env* e) {
    if (pt->type == TREE) {
        struct Object* fn = run(pt->branches->tree, e);
        // TODO: add error handling to check if is function
        struct ArgList* args;
        if (fn->fn->kind == NORMAL || fn->fn->kind == NATIVE)
            args = branchlist_to_arglist_normal(pt->branches, e)->next;
        else
            args = branchlist_to_arglist_macro(pt->branches)->next;
        return call(fn->fn, args);
    } else if (pt->type == LEAF_SYMBOL)
        return env_get(e, pt->s);
    return parsetree_to_obj(pt);
}

struct ArgList* branchlist_to_arglist_normal(struct BranchList* bl, struct Env* e) {
    struct ArgList* head = malloc(sizeof(struct ArgList));
    struct ArgList* tail = head;
    for (struct BranchList* p = bl; p != NULL; p = p->next) {
        tail->obj = run(p->tree, e);
        if (p->next == NULL)
            tail->next = NULL;
        else {
            tail->next = malloc(sizeof(struct ArgList));
            tail = tail->next;
        }
    }
    return head;
}

struct ArgList* branchlist_to_arglist_macro(struct BranchList* bl) {
    struct ArgList* head = malloc(sizeof(struct ArgList));
    struct ArgList* tail = head;
    for (struct BranchList* p = bl; p != NULL; p = bl->next) {
        tail->obj = parsetree_to_obj(p->tree);
        if (p->next == NULL)
            tail->next = NULL;
        else {
            tail->next = malloc(sizeof(struct ArgList));
            tail = tail->next;
        }
    }
    return head;
}
// Combine two above functions (DRY)


struct Object* call(struct Function* fn, struct ArgList* args) {
    struct Env* here = child(fn->env);
    if (fn->kind == NORMAL || fn->kind == MACRO) {
        for (struct ArgList* argname = fn->args; argname != NULL || args != NULL; argname = argname->next, args = args->next)
            env_bind(here, new_record(argname->argname, args->obj));
        return run(fn->code.code, here);
    } else
        return fn->code.native_fn(args);    // Environment can be accessed through get_current_environment
}