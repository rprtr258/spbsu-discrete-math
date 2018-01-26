#include <stdio.h>
#include <string.h>
#include "filesencode.h"

bool doesFileExist(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL)
        return false;
    fclose(file);
    return true;
}

// TODO: fix one-symbol text
// TODO: save tree compactly with bits delimiters
// TODO: turn into utility
// TODO: BASE64 encoding
// TODO: fix \0 symbol
// TODO: configuration for multi-letters symbols

int main() {
    printf("Put message in \"file.txt\"\n");
    if (!doesFileExist("file.txt")) {
        printf("\"file.txt\" not found\n");
        return 0;
    }
    
    encodeFile("file.txt", "encoded.txt", "codeInfo.txt");
    decodeFile("encoded.txt", "decoded.txt");
    
    printf("Done!\n");
    return 0;
}
