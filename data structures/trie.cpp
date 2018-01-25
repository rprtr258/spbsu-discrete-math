#include <string.h>
#include "trie.h"

unsigned int const alphabet = 256;

struct Node {
    Node **childs = nullptr;
    int terminates = 0;
    int strings = 0;
    
    Node() {
        childs = new Node*[alphabet];
        for (unsigned int i = 0; i < alphabet; i++)
            childs[i] = nullptr;
    }
    
    ~Node() {
        for (unsigned int i = 0; i < alphabet; i++)
            delete childs[i];
        delete[] childs;
    }
};

Trie::Trie() {
    root = new Node();
}

Trie::~Trie() {
    delete root;
}

Trie::addString(const unsigned char *str) {
    Node *tmp = root;
    unsigned int strLength = strlen((char*)str);
    for (unsigned int i = 0; i < strLength; i++) {
        if (tmp->childs[str[i]] == nullptr)
            tmp->childs[str[i]] = new Node();
        tmp->strings++;
        tmp = tmp->childs[str[i]];
    }
    tmp->strings++;
    tmp->termnates++;
}
