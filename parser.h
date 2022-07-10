#pragma once

#include "atomic.h"
#include "lexer.h"

enum ParseTreeType {
    LEAF_INT, LEAF_DOUBLE, LEAF_BOOL, LEAF_STRING, LEAF_SYMBOL, TREE
};

struct BranchList;

struct ParseTree {
    enum ParseTreeType type;
    union {
        int i;
        double d;
        bool b;
        char* s;
        struct BranchList* branches;
    };
};

struct BranchList {
    struct ParseTree* tree;
    struct BranchList* next;
};

// Recursive top-down parser.
struct ParseTree* parse(char** s);
struct ParseTree* parse_tree(char** s, struct ParseTree* tree);  // Initializes `tree` and returns it

// To support special syntaxes like '(asdf) for (quote (asdf)), etc.
struct ParseTree* quote(struct ParseTree* t);
struct ParseTree* quasiquote(struct ParseTree* t);
struct ParseTree* unquote(struct ParseTree* t);

// For debugging purposes
void print_parsetree(struct ParseTree* tree);

// For internal use
void append_to_branch_list(struct BranchList** p, struct ParseTree* t);
struct ParseTree* wrap(struct ParseTree* new, struct ParseTree* whole);
void _print_parsetree(struct ParseTree* tree, char* indent);