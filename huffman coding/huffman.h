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
        HuffmanTree(ByteString str);
	HuffmanTree(ByteString tree, int unsigned const treeSize);
        ~HuffmanTree();
        
        ByteString encode(ByteString str);
        ByteString decode(ByteString str, int unsigned const length);
        
	ByteString asString();
        ByteString getInfo();
        
        void printDebug();
};

