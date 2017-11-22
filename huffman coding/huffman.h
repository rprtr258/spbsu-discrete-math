#pragma once
#include <stdio.h>
#include <utility>
#include "bitvector.h"

struct Node;

// TODO: save tree as binary code

struct HuffmanTree {
    Node *root = nullptr;
    
    HuffmanTree(const unsigned char *str);
    HuffmanTree(const BitVector &tree);
    
    ~HuffmanTree();

    BitVector encode(const unsigned char *str) const;
    unsigned char* decode(const BitVector &code) const;
    
    void saveTree(FILE *file) const;
};
