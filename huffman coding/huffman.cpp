#include <string.h>
#include <stdio.h>
#include <math.h>
#include <utility>
#include <algorithm>
#include <queue>
#include <stack>
#include "huffman.h"
#include "freqtable.h"
#include "bitvector.h"

char const separator = '\7';
unsigned int const alphabet = 256;

struct Node {
    Node *l = nullptr;
    Node *r = nullptr;
    unsigned char symbol = '\0';
    int frequency = -1;
    
    bool isLeaf() {
        return (l == nullptr && r == nullptr);
    }
};

Node* buildTree(const FrequencyTable &ftable) {
    std::queue<Node*> firstQueue;
    std::queue<Node*> secondQueue;
    
    auto getRarestNode = [&]() {
        Node *result = nullptr;
        if (firstQueue.empty()) {
            result = secondQueue.front();
            secondQueue.pop();
        } else if (secondQueue.empty()) {
            result = firstQueue.front();
            firstQueue.pop();
        } else if (secondQueue.front()->frequency < firstQueue.front()->frequency) {
            result = secondQueue.front();
            secondQueue.pop();
        } else {
            result = firstQueue.front();
            firstQueue.pop();
        }
        return result;
    };
    
    for (int i = 0; i < ftable.size; i++) {
        Node *leaf = new Node();
        leaf->symbol = ftable[i].first;
        leaf->frequency = ftable[i].second;
        firstQueue.push(leaf);
    }
    
    for (int i = 0; i < ftable.size - 1; i++) {
        Node *first = getRarestNode();
        Node *second = getRarestNode();
        Node *parent = new Node();
        parent->l = first;
        parent->r = second;
        parent->frequency = first->frequency + second->frequency;
        secondQueue.push(parent);
    }
    
    return getRarestNode();
}

HuffmanTree::HuffmanTree(const unsigned char *str) {
    FrequencyTable ftable(str);
    root = buildTree(ftable);
}

HuffmanTree::HuffmanTree(const BitVector &tree) {
    std::stack<Node*> tmp;
    for (unsigned int i = 0; i < tree.capacity; i++) {
        Node* node = new Node();
        if (tree.data[i] != separator) {
            node->symbol = tree.data[i];
        } else {
            Node *r = tmp.top();
            tmp.pop();
            Node *l = tmp.top();
            tmp.pop();
            node->l = l;
            node->r = r;
        }
        tmp.push(node);
    }
    root = tmp.top();
}

void erase(Node *node) {
    if (node == nullptr)
        return;
    erase(node->l);
    erase(node->r);
    delete node;
}

HuffmanTree::~HuffmanTree() {
    erase(root);
}

int writeCodes(Node *node, BitVector *codes, BitVector &buffer, int const level = 0) {
    if (node->isLeaf()) {
        codes[(unsigned int)node->symbol] = buffer;
        return node->frequency * level;
    }
    int result = 0;
    buffer.pushBack(0);
    result += writeCodes(node->l, codes, buffer, level + 1);
    buffer.popBack();
    buffer.pushBack(1);
    result += writeCodes(node->r, codes, buffer, level + 1);
    buffer.popBack();
    return result;
}

BitVector HuffmanTree::encode(const unsigned char *str) const {
    unsigned int strLength = strlen((const char*)str);
    
    BitVector codes[alphabet];
    
    BitVector buffer;
    writeCodes(root, codes, buffer);
    
    BitVector result;
    for (unsigned int i = 0; i < strLength; i++)
        result.pushBack(codes[(unsigned int)str[i]]);    
    return result;
}

void putChar(unsigned char *&str, int &size, const unsigned char &c) {
    unsigned char *newStr = new unsigned char[size + 1];
    if (str != nullptr) {
        memcpy(newStr, str, size);
        delete[] str;
    }
    newStr[size] = c;
    
    size++;
    str = newStr;
}

unsigned char* HuffmanTree::decode(const BitVector &code) const {
    unsigned char *res = nullptr;
    int size = 0;
    Node *tmp = root;
    for (unsigned int i = 0; i < code.size; i++) {
        bool bit = code.get(i);
        tmp = (!bit ? tmp->l : tmp->r);
        if (tmp->isLeaf()) {
            putChar(res, size, tmp->symbol);
            tmp = root;
        }
    }
    putChar(res, size, '\0');
    return res;
}

void debug(Node *node, FILE *file) {
    if (node == nullptr)
        return;
    
    if (node->isLeaf()) {
        fputc(node->symbol, file);
    } else {
        debug(node->l, file);
        debug(node->r, file);
        fputc(separator, file);
    }
}

void HuffmanTree::saveTree(FILE *file) const {
    debug(root, file);
    fputc((char)0, file);
}
