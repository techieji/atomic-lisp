#include "parser.h"

struct ParseTree* parse(char** tape) {
    // Why is this function accepting a char** and not
    // struct Token[]? Because it's more convenient
    // (and memory efficient) to generate the tokens 
    // on demand.
    struct Token* current = get_next_token(tape);
    struct ParseTree* tree = malloc(sizeof(struct ParseTree));
    switch (current->type) {
        case TOKEN_INT:
            tree->type = LEAF_INT;
            tree->i = current->i;
            break;
        case TOKEN_DOUBLE:
            tree->type = LEAF_DOUBLE;
            tree->d = current->d;
            break;
        case TOKEN_BOOL:
            tree->type = LEAF_BOOL;
            tree->b = current->b;
            break;
        case TOKEN_STRING:
            tree->type = LEAF_STRING;
            tree->s = current->s;
            break;
        case TOKEN_SYMBOL:
            tree->type = LEAF_SYMBOL;
            tree->s = current->s;
            break;
        case TOKEN_TERMINAL:
            switch (current->c) {    // TODO: Simplify code
                case '(':
                    tree = parse_tree(tape, tree);
                    break;
                case '\'':
                    tree = quote(parse(tape));
                    break;
                case '`':
                    tree = quasiquote(parse(tape));
                    break;
                case ',':
                    tree = unquote(parse(tape));
                    break;
                default:;
                    puts("ERROR on parser.c:38");
                    // TODO: Add error handling
            }
            break;
        case TOKEN_EOF:
            puts("EOF reached too soon?"); // TODO: Add error handling
            exit(1);
        default:
            return NULL;
    }
    free(current);
    return tree;
}

struct ParseTree* parse_tree(char** tape, struct ParseTree* tree) {
    struct Token* next;
    tree->type = TREE;
    tree->branches = NULL;
    next = get_next_token(tape);
    while (!(next->type == TOKEN_TERMINAL && next->c == ')')) {
        pushback(next);
        append_to_branch_list(&tree->branches, parse(tape));
        next = get_next_token(tape);    // Repetition of line above
    }
    return tree;
}

void append_to_branch_list(struct BranchList** p, struct ParseTree* t) {
    if (*p == NULL) {
        *p = malloc(sizeof(struct BranchList));
        (*p)->tree = t;
        (*p)->next = NULL;
    } else {
        struct BranchList* loop;
        for (loop = *p; loop->next != NULL; loop = loop->next);
        loop->next = malloc(sizeof(struct BranchList));
        loop->next->tree = t;
        loop->next->next = NULL;
    }
}

struct ParseTree* wrap(struct ParseTree* new, struct ParseTree* whole) {
    struct ParseTree* ret = malloc(sizeof(struct ParseTree));
    ret->type = TREE;
    ret->branches = malloc(sizeof(struct BranchList));
    ret->branches->tree = new;
    ret->branches->next = malloc(sizeof(struct BranchList));
    ret->branches->next->tree = whole;
    ret->branches->next->next = NULL;
    return ret;
}

struct ParseTree* quote(struct ParseTree* t) {
    struct ParseTree* node = malloc(sizeof(struct ParseTree));
    node->type = LEAF_SYMBOL;
    node->s = "quote";
    return wrap(node, t);
}

struct ParseTree* quasiquote(struct ParseTree* t) {
    struct ParseTree* node = malloc(sizeof(struct ParseTree));
    node->type = LEAF_SYMBOL;
    node->s = "quasiquote";
    return wrap(node, t);
}

struct ParseTree* unquote(struct ParseTree* t) {
    struct ParseTree* node = malloc(sizeof(struct ParseTree));
    node->type = LEAF_SYMBOL;
    node->s = "unquote";
    return wrap(node, t);
}

void print_parsetree(struct ParseTree* tree) {
    _print_parsetree(tree, "");
}

void _print_parsetree(struct ParseTree* tree, char* indent) {
    switch (tree->type) {
        case LEAF_INT:
            printf("%sINT    %d\n", indent, tree->i);
            return;
        case LEAF_DOUBLE:
            printf("%sDOUBLE    %g\n", indent, tree->d);
            return;
        case LEAF_BOOL:
            printf("%sBOOL    %s", indent, tree->b ? "#t" : "#f");
            return;
        case LEAF_STRING:
            printf("%sSTRING    \"%s\"\n", indent, tree->s);
            return;
        case LEAF_SYMBOL:
            printf("%sSYMBOL    %s\n", indent, tree->s);
            return;
        case TREE:
            _print_parsetree(tree->branches->tree, indent);
            char* new_indent = malloc((2 + strlen(indent)) * sizeof(char));
            sprintf(new_indent, "  %s", indent);
            for (struct BranchList* bl = tree->branches->next; bl != NULL; bl = bl->next) {
                _print_parsetree(bl->tree, new_indent);
            }
            free(new_indent);
    }
}