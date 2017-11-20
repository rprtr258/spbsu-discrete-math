#pragma once
#include <stdio.h>
#include <utility>
#include "bitvector.h"

struct Node;

// TODO: save tree as binary code

struct HuffmanTree {
    Node *root = nullptr;
    
    HuffmanTree(const char *str);
    HuffmanTree(const BitVector &tree);
    
    ~HuffmanTree();

    BitVector encode(const char *str) const;
    char* decode(const BitVector &code) const;
    
    void saveTree(FILE *file) const;
    void saveInfo(int const codeLength, int const textLength, FILE *file) const;
};
