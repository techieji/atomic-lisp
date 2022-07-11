#pragma once

#include "objects.h"
#include "runtime.h"

#define DECLARE(fnname) struct Object* fnname (struct ArgList* al)

void load_stl(struct Env* e);

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
DECLARE(display);
DECLARE(newline);
// DECLARE(the_environment);        // Implement after environment is struct Object*