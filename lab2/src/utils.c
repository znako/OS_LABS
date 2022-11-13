#include <utils.h>

char* ReadString(FILE* stream) 
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

char* ReadStringWithoutVowels(FILE* stream) 
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

    char* vowels = {"AEIOUYaeiouy"};

    while ((readChar = getc(stream)) != EOF) 
    {
        int isVowel = 0;

        for (int i = 0; i < (int)strlen(vowels); ++i){
            if (readChar == vowels[i]){
                isVowel = 1;
            }
        }

        if (isVowel == 0){
            buffer[idx++] = readChar;
        }

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