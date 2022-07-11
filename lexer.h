#pragma once

#include "atomic.h"

enum TokenType {
    TOKEN_INT, TOKEN_DOUBLE, TOKEN_BOOL, TOKEN_STRING, TOKEN_SYMBOL, TOKEN_TERMINAL, TOKEN_EOF
};

struct Token {
    enum TokenType type;
    union {
        int i;                   // Used for TOKEN_INT. Could be converted into a pointer to save memory.
        bool b;                  // Used for TOKEN_BOOL.
        double d;                // Used for TOKEN_DOUBLE. Same point as above, but more so.
        char* s;                 // Used for both TOKEN_STRING and TOKEN_SYMBOL
        char c;                  // Used for TOKEN_TERMINAL; examples: `(`, `)`, `'`
    };
};


struct Token* get_next_token(char** tape);  // Note: after lex is done, will return infinite stream of EOFs

void pushback(struct Token* tok);     // To enable look-ahead of 1 token. See parser.c for use.

void print_token(struct Token* tok);    // For debugging purposes