#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* ReadString(FILE* stream);
char* ReadStringWithoutVowels(FILE* stream);

#endif