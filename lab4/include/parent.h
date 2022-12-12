#ifndef PARENT_H
#define PARENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

void ParentRoutine(FILE* input);
char* ReadString(FILE* stream);

#endif