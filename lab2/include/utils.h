#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int(*TFilter)(char);

char* ReadString(FILE* stream, TFilter filter);
int FilterNone(char c);
int FilterVowels(char c);

#endif