#include <stdio.h>
#include <string.h>
#include "decoder.h"

// TODO: make encoding without loading file into memory and test on large text
// TODO: fix one-symbol text
// TODO: save tree compactly with bits delimiters
// TODO: read encoded in decode() with readFile
// TODO: turn into utility
// TODO: optimize memory allocating

int main() {    
    encode("dontopen.txt", "encoded.txt");
    decode("encoded.txt", "decoded.txt");
    
    return 0;
}