#pragma once
#include <stdio.h>
#include <utility>
#include <vector>
#include "node.h"

typedef std::vector<char unsigned> ByteString;

class HuffmanTree {
    private:
        Node *root = nullptr;
    public:
        HuffmanTree(char const *filename);
        HuffmanTree(FILE *file, int unsigned const treeSize);
        ~HuffmanTree();
        
        void encode(char const *filename, FILE *outFile);
        void decode(FILE *fileIn, char const *filename, int unsigned const length);
        
        int unsigned getResultLength();
        ByteString asString(int unsigned &length);
};

