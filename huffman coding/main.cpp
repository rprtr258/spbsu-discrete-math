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
//       add 2 symbols at the end & remove them in decoding
// TODO: check empty file encoding
// TODO: turn into utility
// TODO: BASE64 encoding

int main() {
    printf("Put message in \"file.txt\"\n");
    if (!doesFileExist("file.txt")) {
        printf("\"file.txt\" not found\n");
        return 0;
    }
    
    encodeFile("file.txt", "encoded.txt");
    decodeFile("encoded.txt", "decoded.txt");
    
    printf("Done!\n");
    return 0;
}
