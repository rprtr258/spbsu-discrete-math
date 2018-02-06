#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include "filesencode.h"
#include "huffman.h"

typedef std::vector<char unsigned> ByteString;

int unsigned const charMemory = (1 << (8 * sizeof(char unsigned)));
int unsigned const treeSizeLength = 2;
int unsigned const textSizeLength = sizeof(int unsigned);

ByteString readFile(const char *filename) {
    FILE *file = fopen(filename, "rb");
    ByteString result;
    
    char symbol = '\0';
    fscanf(file, "%c", &symbol);
    while (!feof(file)) {
        result.push_back(symbol);
        fscanf(file, "%c", &symbol);
    }
    
    fclose(file);
    return result;
}

void writeByteString(ByteString const &string, FILE *file) {
    for (char unsigned const &byte : string)
        fprintf(file, "%c", byte);
}

ByteString getAsBytes(int unsigned num, int unsigned const requiredSize) {
    ByteString result;
    while (num) {
        result.push_back(num % charMemory);
        num /= charMemory;
    }
    while (result.size() < requiredSize)
        result.push_back(0);
    std::reverse(result.begin(), result.end());
    return result;
}

void encodeFile(const char *fileInput, const char *fileOutput) {
    HuffmanTree tree(fileInput);
    int unsigned treeLength = 0;
    ByteString savedTree = tree.asString(treeLength);
    ByteString treeSize = getAsBytes(treeLength, treeSizeLength);
    int unsigned encodedLength = tree.getResultLength();
    ByteString textSize = getAsBytes(encodedLength, textSizeLength);
    
    FILE *outFile = fopen(fileOutput, "wb");
    writeByteString(treeSize, outFile);
    writeByteString(textSize, outFile);
    writeByteString(savedTree, outFile);
    tree.encode(fileInput, outFile);
    
    fclose(outFile);
}

void decodeFile(const char *fileInput, const char *fileOutput) {
    ByteString file = readFile(fileInput);
    int unsigned treeLength = 0;
    for (int unsigned i = 0; i < treeSizeLength; i++)
        treeLength = treeLength * charMemory + file[i];
    int unsigned encodedLength = 0;
    for (int unsigned i = 0; i < textSizeLength; i++)
        encodedLength = encodedLength * charMemory + file[i + treeSizeLength];
    
    file.erase(file.begin(), file.begin() + treeSizeLength + textSizeLength);
    HuffmanTree tree(file, treeLength);
    file.erase(file.begin(), file.begin() + (treeLength + 7) / 8);
    
    ByteString text = tree.decode(file, encodedLength);
    
    FILE *fileOut = fopen(fileOutput, "wb");
    writeByteString(text, fileOut);
    fclose(fileOut);
}
