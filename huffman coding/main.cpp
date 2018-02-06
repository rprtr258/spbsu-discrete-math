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

// TODO: fix bits
// TODO: fix one-symbol text
//       add 2 symbols at the end & remove them in decoding
// TODO: turn into utility
// TODO: BASE64 encoding
// TODO: configuration for multi-letters symbols
// TODO: encode/decode without reading file in RAM
/* TODO: encode format:
 *       (size of tree in bits), (size of encoded text), (encoded tree), (encoded text)
 *       tree format:
 *       (0(8bit char)) or (1) - separator
 */

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
