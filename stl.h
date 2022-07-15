#pragma once

#include "objects.h"
#include "runtime.h"

#define DECLARE(fnname) struct Object* fnname (struct ArgList* al)
#define EXPOSE(kind, fn_name, exposed_name) \
                fn = make_fn( kind ); \
                fn->fn->code.native_fn = fn_name; \
                env_bind(e, new_record(exposed_name, fn));

void load_stl(struct Env* e);
struct Object* make_fn(enum Functionkind kind);

/////////////////////
//  Special forms  //
/////////////////////

// Taken from http://web.archive.org/web/20131010070904/http://www.gnu.org/software/mit-scheme/documentation/mit-scheme-ref/Special-Forms.html#Special-Forms

DECLARE(lambda);
DECLARE(let);      // Semantics of let*; semantics of execution allow it to behave like let and letrec
DECLARE(define);   // Turn into a normal macro later
DECLARE(quote_);        // After parse trees are implemented as Objects, turn into a normal macro
DECLARE(quasiquote_);   // Same as above
DECLARE(if_);
DECLARE(cond);       // Turn into a normal macro later
DECLARE(begin);      // Why isn't this a native function? Because it creates a new environment
DECLARE(defmacro);

//////////////////////
// Native Functions //
//////////////////////

DECLARE(and);
DECLARE(or);
DECLARE(car);
DECLARE(cdr);
DECLARE(display);
DECLARE(newline);
DECLARE(plus);
DECLARE(exit_);
DECLARE(display_variables);
// DECLARE(the_environment);        // Implement after environment is struct Object*