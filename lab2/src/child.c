#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "utils.h"

int main(int argc, char* argv[])
{
    if (argc < 1)
    {
        perror("too few arguments )");
        exit(EXIT_FAILURE);
    }

    if (strlen(argv[0]) < 1)
    {
        perror("too few arguments )");
        exit(EXIT_FAILURE);
    }

    char* strInput;

    while ((strInput = ReadStringWithoutVowels(stdin)) != NULL)
    {
        write(1, strInput, strlen(strInput));
        free(strInput);
    }

    return 0;
}