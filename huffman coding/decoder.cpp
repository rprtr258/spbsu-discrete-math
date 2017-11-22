#include <string.h>
#include "decoder.h"
#include "huffman.h"
#include "bitvector.h"

void put(unsigned char *&str, int &size, const unsigned char &c) {
    unsigned char *newStr = new unsigned char[size + 1];
    if (str != nullptr) {
        memcpy(newStr, str, size);
        delete[] str;
    }
    newStr[size] = c;
    
    size++;
    str = newStr;
}

unsigned char* readFile(const char *filename) {
    FILE *file = fopen(filename, "rb");
    unsigned char *buffer = nullptr;
    int symbol = '$';
    int i = 0;
    symbol = fgetc(file);
    while (symbol != EOF) {
        put(buffer, i, (char)symbol);
        symbol = fgetc(file);
    }
    put(buffer, i, '\0');
    
    fclose(file);
    return buffer;
}

void encode(const char *inputFile, const char *outputFile) {
    unsigned char *str = readFile(inputFile);
    HuffmanTree tree(str);
    FILE *outFile = fopen(outputFile, "wb");
    
    BitVector codeString = tree.encode(str);
    
    tree.saveTree(outFile);
    fprintf(outFile, "%d\n", codeString.size);
    for (unsigned int i = 0; i < codeString.capacity; i++)
        fputc(codeString.data[i], outFile);
    
    fclose(outFile);
    delete[] str;
}

void decode(const char *inputFile, const char *outputFile) {
    FILE *inFile = fopen(inputFile, "rb");
    FILE *outFile = fopen(outputFile, "wb");
    
    int c = '$';
    unsigned char *str = nullptr;
    int j = 0;
    
    while (j < 2 || str[j - 1] != (char)0) {
        c = fgetc(inFile);
        put(str, j, c);
    }
    BitVector treeEncoded;
    treeEncoded.data = str;
    treeEncoded.size = (j - 1) * 8;
    treeEncoded.capacity = j - 1;
    
    HuffmanTree tree(treeEncoded);
    
    int codeLength = -1;
    fscanf(inFile, "%d", &codeLength);
    
    str = nullptr;
    j = 0;
    c = fgetc(inFile); // end of line
    while (c != EOF) {
        c = fgetc(inFile);
        if (c == EOF)
            continue;
        put(str, j, c);
    }
    BitVector code;
    code.data = str;
    code.size = codeLength;
    code.capacity = j;
    str = tree.decode(code);
    
    fputs((const char *)str, outFile);
    
    printf("Decode done!\n");
    
    delete[] str;
    fclose(inFile);
    fclose(outFile);
}
