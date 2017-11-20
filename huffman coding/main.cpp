#include <stdio.h>
#include <string.h>
#include "huffman.h"
#include "bitvector.h"

char* put(char *str, int &size, char c) {
    char *newStr = new char[size + 1];
    if (str != nullptr) {
        memcpy(newStr, str, size);
        delete[] str;
    }
    newStr[size] = c;
    
    size++;
    return newStr;
}

char* readFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    char *buffer = nullptr;
    char symbol = '\0';
    int i = 0;
    while (!feof(file)) {
        fscanf(file, "%c", &symbol);
        if (feof(file))
            continue;
        buffer = put(buffer, i, symbol);
    }
    buffer = put(buffer, i, '\0');
    // printf("read: %s\n", buffer);
    char *result = new char[i + 1];
    strcpy(result, buffer);
    
    fclose(file);
    return result;
}

void encode() {
    char *str = readFile("dontopen.txt");
    HuffmanTree tree(str);
    FILE *outFile = fopen("encoded.txt", "w");
    FILE *infoFile = fopen("codeInfo.txt", "w");
    
    BitVector codeString = tree.encode(str);
    
    tree.saveInfo(codeString.size, strlen(str), infoFile);
    
    tree.saveTree(outFile);
    fprintf(outFile, "%d\n", codeString.size);
    for (unsigned int i = 0; i < codeString.capacity; i++) {
        fprintf(outFile, "%c", (codeString.data[i] == '' ? '\0' : codeString.data[i]));
        // printf("%c", codeString.data[i]);
    }
    // printf("\n");
    
    // printf("Encoded: \n");
    // codeString.printBytes();
    // printf("\n");
    
    printf("Done!\n");
    
    fclose(outFile);
    fclose(infoFile);
    delete[] str;
}

void decode() {
    FILE *inFile = fopen("encoded.txt", "r");
    FILE *outFile = fopen("decoded.txt", "w");
    
    char c = '\0';
    char *str = nullptr;
    int j = 0;
    
    while (j < 2 || (str[j - 1] != '\n' || str[j - 2] != '\n')) {
        fscanf(inFile, "%c", &c);
        // printf("%c", c);
        str = put(str, j, c);
    }
    BitVector treeEncoded;
    treeEncoded.data = str;
    treeEncoded.size = (j - 2) * 8;
    treeEncoded.capacity = j - 2;
    
    HuffmanTree tree(treeEncoded);
    
    int codeLength = -1;
    fscanf(inFile, "%d", &codeLength);
    
    str = nullptr;
    j = 0;
    fscanf(inFile, "%c", &c);
    while (!feof(inFile)) {
        fscanf(inFile, "%c", &c);
        if (feof(inFile))
            continue;
        c = (c == '\0' ? '' : c);
        // printf("%c", c);
        str = put(str, j, c);
    }
    // printf("\n");
    BitVector code;
    code.data = str;
    code.size = codeLength;
    code.capacity = j;
    str = tree.decode(code);
    
    // printf("Read: \n");
    // code.printBytes();
    // printf("\n");
    
    fprintf(outFile, "%s", str);
    
    printf("Decode done!\n");
    
    delete[] str;
    fclose(inFile);
    fclose(outFile);
}

// TODO: make encoding without loading file into memory and test on large text
// TODO: fix russian symbols encoding(code >=128)
// TODO: fix one-symbol text

int main() {
    printf("Don\'t open \"dontopen.txt\", here is secret info that will be encoded\n");
    
    encode();
    decode();
    
    return 0;
}