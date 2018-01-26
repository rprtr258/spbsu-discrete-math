#include <stdio.h>
#include <string.h>
#include "filesencode.h"
#include "huffman.h"
#include "putChar.h"

char* readFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    char *result = new char[1];
    result[0] = '\0';
    
    char symbol = '\0';
    fscanf(file, "%c", &symbol);
    while (!feof(file)) {
        putChar(result, symbol);
        fscanf(file, "%c", &symbol);
    }
    
    fclose(file);
    return result;
}

int encodeFile(const char *fileInput, const char *fileOutput, const char *fileInfo) {
    char *str = readFile(fileInput);
    
    HuffmanTree *tree = createTree(str);
    FILE *outFile = fopen(fileOutput, "w");
    char *codeString = encode(tree, str);
    
    FILE *infoFile = fopen(fileInfo, "w");
    saveInfo(tree, infoFile, strlen(str));
    
    saveTree(tree, outFile);
    fprintf(outFile, "%s", codeString);
    
    fclose(outFile);
    fclose(infoFile);
    delete[] codeString;
    delete[] str;
    deleteTree(tree);
    return 0;
}

int decodeFile(const char *fileInput, const char *fileOutput) {
    HuffmanTree *tree = readTree(fileInput);
    char *text = decodeFile(tree, fileInput);
    
    FILE *fileOut = fopen(fileOutput, "w");
    fprintf(fileOut, "%s", text);
    
    deleteTree(tree);
    delete[] text;
    fclose(fileOut);
    return 0;
}
