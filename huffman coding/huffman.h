#pragma once
#include <stdio.h>
#include <utility>
#include "node.h"

class HuffmanTree {
    private:
        Node *root = nullptr;
    public:
        HuffmanTree(const char *str);
        ~HuffmanTree();
        
        char* encode(const char *str);
        char* decode(const char *str);
        
        char* asString();
        char* getInfo();
        
        void printDebug();
};

