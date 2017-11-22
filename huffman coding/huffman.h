#pragma once
#include <stdio.h>
#include <utility>
#include "bitvector.h"

struct Node;

struct HuffmanTree {
    Node *root = nullptr;
    
    HuffmanTree();
    HuffmanTree(const char *filename);
    HuffmanTree(const BitVector &tree);
    
    ~HuffmanTree();

    void encode(const char *inputFile, const char *outputFile) const;
    void decode(const char *inputFile, const char *outputFile);
    
    void saveTree(FILE *file) const;
};
