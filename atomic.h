#pragma once

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdbool.h>

// Main function

void repl(void);

// Debugging functions

void test_lexer(char* s);
void test_parser(char* s);
void test_objects(char* s);
void test_runtime_and_stl(char* s);