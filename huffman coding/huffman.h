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
        HuffmanTree(ByteString tree, int unsigned const treeSize);
        ~HuffmanTree();
        
        ByteString encode(char const *filename);
        ByteString decode(ByteString str, int unsigned const length);
        
        int unsigned getResultLength();
        ByteString asString(int unsigned &length);
};

