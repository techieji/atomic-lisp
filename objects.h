#pragma once

#include <string.h>
#include "parser.h"
#include "atomic.h"

struct Pair;
struct Function;
struct Env;

/////////////////////////
//   Actual objects    //
/////////////////////////

enum ObjectType {
    INT, DOUBLE, BOOL, STRING, SYMBOL, PAIR, NIL, FUNCTION
};

struct Object {
    enum ObjectType type;
    union {
        int i;
        double d;
        bool b;
        char* s;
        struct Pair* pair;
        struct Function* fn;
    };
};

struct Pair {
    struct Object* car;
    struct Object* cdr;
};

struct ArgList {          // Can be used for both arg names and arg values
    union {
        char* argname;
        struct Object* obj;
    };
    struct ArgList* next;
};

enum FunctionKind {
    NORMAL, MACRO,        // Both of these are written by the user in Atomic Lisp
    NATIVE, SPECIAL       // Both of these are written in C
//  ^^^^^^ Both of these evaluate their arguments before running;
//          ^^^^^^^ Both of these get their arguments as ParseTrees
};

struct Function {
    enum FunctionKind kind;
    struct ArgList* args;
    struct Env* env;        // Needed for closures
    union FunctionCode {
        struct ParseTree* code;
        struct Object* (*native_fn)(struct ArgList* al);    // native_fn is a function ptr accepting an arglist and returning an object
        // If writing a special function or a function that otherwise needs to get the environment, use the get_current_environment function below.
    } code;
};

struct Object* parsetree_to_obj(struct ParseTree* pt);     // Used for implementing macros: parsetrees have to be converted into lists
struct ParseTree* obj_to_parsetree(struct Object* o);      // Again, used in macros, to convert those lists back into executable parsetrees
void print_obj(struct Object* o);         // Used in the REPL to print out expression values

// Utils for above
struct Object* branchlist_to_obj(struct BranchList* bl);

/////////////////////////
//     Environments    //
/////////////////////////

struct Record {       // Hmmmm, gives me an idea. What about VCed variables?
    char* varname;
    struct Object* obj;
};

struct Scope {
    struct Record* rec;
    struct Scope* next;
};

struct Env {
    struct Scope* scope;
    struct Env* upper;
};

struct Record* new_record(char* varname, struct Object* obj);
void env_bind(struct Env* e, struct Record* record);
struct Object* env_get(struct Env* e, char* varname);
struct Env* child(struct Env* parent);          // Creates a new environment with `parent` as the upper scope
void pop_env(void);
struct Env* get_current_environment(void);

///////////////////////
// Optional Features //
///////////////////////
// The below functions help manage memory, so if you don't want
// to slap on a garbage collector, you can define and use these functions
// to help keep memory usage down.

// void freeze(struct Env* current);        // Freezes the current environment so that closures work.
// void unfreeze(struct Env* current);      // Unfreezes the environment. Should be used when freeing functions.
// void prune(struct Env* current);
// void free_env(struct Env* e);