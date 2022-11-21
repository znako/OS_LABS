#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int(*TFilter)(char);

char* ReadString(FILE* stream, TFilter filter);
int FilterZero(char c);
int Filter(char c);

#endif