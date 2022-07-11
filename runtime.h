#pragma once

#include "parser.h"
#include "objects.h"

struct Object* run(struct ParseTree* pt, struct Env* e);

// Internal utils
struct ArgList* branchlist_to_arglist_normal(struct BranchList* bl, struct Env* e);
struct ArgList* branchlist_to_arglist_macro(struct BranchList* bl);
struct Object* call(struct Function* fn, struct ArgList* args);