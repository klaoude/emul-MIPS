#ifndef REGEX_H
#define REGEX_H

#include <regex.h>
#include <string.h>

#include "Stack.h"

Stack* regex_match(char* str, char* reg);

#endif