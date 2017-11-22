#include <string.h>
#include "decoder.h"
#include "huffman.h"

void encode(const char *inputFile, const char *outputFile) {
    HuffmanTree tree(inputFile);
    tree.encode(inputFile, outputFile);
}

void decode(const char *inputFile, const char *outputFile) {
    HuffmanTree tree;
    tree.decode(inputFile, outputFile);
}
