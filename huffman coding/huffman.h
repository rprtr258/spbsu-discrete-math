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
        
        ByteString encode(char const *filename, int unsigned &length);
        ByteString decode(ByteString str, int unsigned const length);
        
        ByteString asString(int unsigned &length);
};

