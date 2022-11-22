#include <utils.h>


char* ReadString(FILE* stream, TFilter filter) 
{
    if(feof(stream)) {
        return NULL;
    }

    const int chunkSize = 256;
    char* buffer = (char*)malloc(chunkSize);
    int bufferSize = chunkSize;

    if(buffer == NULL) 
    {
        printf("Couldn't allocate buffer");
        exit(EXIT_FAILURE);
    }

    int readChar;
    int idx = 0;

    while ((readChar = getc(stream)) != EOF) 
    {
        if(filter(readChar)){
            continue;
        }
        buffer[idx++] = readChar;

        if (idx == bufferSize) 
        {
            buffer = realloc(buffer, bufferSize + chunkSize);
            bufferSize += chunkSize;
        }

        if (readChar == '\n') {
            break;
        }
    }

    buffer[idx] = '\0';

    return buffer;
}

int FilterNone(char c)
{
    return c-c;
}

int FilterVowels(char c)
{
    char* vowels = {"AEIOUYaeiouy"};

    for (int i = 0; i < (int)strlen(vowels); ++i)
    {
        if (c == vowels[i]){
            return 1;
        }
    }
    return 0;
}
