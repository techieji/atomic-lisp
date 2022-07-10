#pragma once

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

struct ArgList {
    char* argname;
    struct ArgList* next;
};

struct Function {
    struct ArgList* args;
    struct ParseTree* code;
    struct Env* env;        // Needed for closures
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
    int referred;
    struct Scope* scope;
    struct Env* upper;
};

struct Record* new_record(char* varname, struct Object* obj);
void env_bind(struct Env* e, struct Record* record);
struct Object* env_get(struct Env* e, char* varname);
struct Env* child(struct Env* parent);          // Creates a new environment with `parent` as the upper scope
void freeze(struct Env* current);        // Freezes the current environment so that closures work.
void unfreeze(struct Env* current);      // Unfreezes the environment. Should be used when freeing functions.