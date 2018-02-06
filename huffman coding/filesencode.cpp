#include <stdio.h>
#include <string.h>
#include <vector>
#include "filesencode.h"
#include "huffman.h"
#include "putChar.h"

typedef std::vector<char unsigned> ByteString;

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

void encodeFile(const char *fileInput, const char *fileOutput, const char *fileInfo) {
    ByteString str = readFile(fileInput);
    
    HuffmanTree tree(str);
    ByteString encodedString = tree.encode(str);
    ByteString savedTree = tree.asString();
    
    FILE *infoFile = fopen(fileInfo, "w");
    ByteString treeInfo = tree.getInfo();
    writeByteString(treeInfo, infoFile);
    fclose(infoFile);
    
    FILE *outFile = fopen(fileOutput, "wb");
    fprintf(outFile, "%03u", savedTree.size());
    writeByteString(savedTree, outFile);
    fprintf(outFile, "%u\n", encodedString.size());
    writeByteString(encodedString, outFile);
    fclose(outFile);
}

int unsigned getTextLength(ByteString &encoded) {
    int unsigned treeSize = (encoded[0] - '0') * 100 + (encoded[1] - '0') * 10 + (encoded[2] - '0');
    int unsigned result = 0;
    int unsigned i = 3 + treeSize;
    while (encoded[i] != '\n') {
        result = result * 10 + (encoded[i] - '0');
        i++;
    }
    encoded.erase(encoded.begin(), encoded.begin() + i + 1);
    return result;
}

void decodeFile(const char *fileInput, const char *fileOutput) {
    ByteString file = readFile(fileInput);
    int unsigned treeSize = (file[0] - '0') * 100 + (file[1] - '0') * 10 + (file[2] - '0');
    HuffmanTree tree(file, treeSize);

    int unsigned encodedLength = getTextLength(file);
    ByteString text = tree.decode(file, encodedLength);
    
    FILE *fileOut = fopen(fileOutput, "wb");
    writeByteString(text, fileOut);
    fclose(fileOut);
}
