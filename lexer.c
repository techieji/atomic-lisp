#include "lexer.h"

static struct Token* _pushed_back = NULL;

struct Token* get_next_token(char** tape) {
    struct Token* tok;
    if (_pushed_back != NULL) {
        tok = _pushed_back;
        _pushed_back = NULL;
        return tok;
    }
    tok = malloc(sizeof(struct Token));
    while (isspace(**tape)) (*tape)++;   // Eat up whitespace
    if (**tape == '\0') {
        tok->type = TOKEN_EOF;
        return tok;
    }
    if (**tape == '#') {
        char c = *++*tape;
        tok->type = TOKEN_BOOL;
        tok->b = c == 't';      // TODO: Make more robust (#r becomes false)
        (*tape)++;
        return tok;
    }
    if (strchr("()'`,", **tape) != NULL) {
        // Handle all terminals.
        tok->type = TOKEN_TERMINAL;
        tok->c = *(*tape)++;
        return tok;
    }
    if (**tape == '"') {
        // Handles string by finding length of string and copying over to tok
        // TODO: handle escape sequences
        int i;
        tok->type = TOKEN_STRING;
        (*tape)++;
        for (i = 0; *(*tape + i) != '"'; i++);
        tok->s = malloc(i * sizeof(char));    // i is going to be up by one, so no need to allocate for null byte
        strncpy(tok->s, *tape, i);
        *(tok->s + i) = '\0';
        *tape += i + 1;    // To correct for the pointer landing on the close quote
        return tok;
    }
    if (isdigit(**tape) || **tape == '-') {
        // Could be start of double or integer... but could also be a function
        bool isint = true;
        int i;
        char* cur;
        // To detect whether the number is a double or an integer
        for (i = 0, cur = *tape; isdigit(*cur) || *cur == '.' || *cur == '-'; i++, cur++)
            if (*cur == '.')
                isint = false;
        if (i == 1  && **tape == '-')
            // Only `-` was parsed, so is a function
            goto parse_symbol;
        if (isint) {
            tok->type = TOKEN_INT;
            tok->i = atoi(*tape);
        } else {
            tok->type = TOKEN_DOUBLE;
            tok->d = atof(*tape);
        }
        *tape += i;
        return tok;
    }
parse_symbol:;
    // Must be a symbol (varname)
    int i;
    for (i = 0; strchr(" ()'`,#", *(*tape + i)) == NULL; i++);
    tok->type = TOKEN_SYMBOL;
    tok->s = malloc((i + 1) * sizeof(char));
    strncpy(tok->s, *tape, i);
    *(tok->s + i + 1) = '\0';
    *tape += i;
    return tok;
}

void pushback(struct Token* tok) {
    _pushed_back = tok;
}

void print_token(struct Token* tok) {
    switch (tok->type) {
        case TOKEN_INT:
            printf("INT\t\t%d", tok->i);
            break;
        case TOKEN_DOUBLE:
            printf("DOUBLE\t\t%g", tok->d);
            break;
        case TOKEN_BOOL:
            printf("BOOL\t\t%s", tok->b ? "#t" : "#f");
            break;
        case TOKEN_STRING:
            printf("STRING\t\t\"%s\"", tok->s);
            break;
        case TOKEN_SYMBOL:
            printf("SYMBOL\t\t%s", tok->s);
            break;
        case TOKEN_TERMINAL:
            printf("TERMINAL\t`%c`", tok->c);
            break;
        case TOKEN_EOF:
            printf("EOF");
            break;
        default:
            printf("UNKNOWN");
    }
    printf("\n");
}