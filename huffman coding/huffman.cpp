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
    char symbol = '\0';
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

HuffmanTree::HuffmanTree(const char *str) {
    FrequencyTable ftable(str);
    root = buildTree(ftable);
}

HuffmanTree::HuffmanTree(const BitVector &tree) {
    std::stack<Node*> tmp;
    for (unsigned int i = 0; i < tree.size / 8; i++) {
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
        
        printf("%c(%X): ", node->symbol, node->symbol);
        for (unsigned int i = 0; i < buffer.size; i++)
            printf("%c", buffer.get(i) ? '1' : '0');
        printf("\n");
        
        // if (node->symbol == '\n')
            // fprintf(file, "\'\\n\'(ASCII code: %02X): frequency: %3d, code: %s\n", node->symbol, node->frequency, buffer);
        // else
            // fprintf(file, "\'%c\' (ASCII code: %02X): frequency: %3d, code: %s\n", node->symbol, node->symbol, node->frequency, buffer);
        
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

BitVector HuffmanTree::encode(const char *str) const {
    unsigned int strLength = strlen(str);
    
    BitVector codes[alphabet];
    
    BitVector buffer;
    writeCodes(root, codes, buffer);
    
    BitVector result;
    for (unsigned int i = 0; i < strLength; i++)
        result.pushBack(codes[(unsigned int)str[i]]);    
    return result;
}

char* putChar(char *str, int &size, char c) {
    char *newStr = new char[size + 1];
    if (str != nullptr) {
        memcpy(newStr, str, size);
        delete[] str;
    }
    newStr[size] = c;
    
    size++;
    return newStr;
}

char* HuffmanTree::decode(const BitVector &code) const {
    char *res = nullptr;
    int size = 0;
    Node *tmp = root;
    for (unsigned int i = 0; i < code.size; i++) {
        bool bit = code.get(i);
        tmp = (!bit ? tmp->l : tmp->r);
        if (tmp->isLeaf()) {
            res = putChar(res, size, tmp->symbol);
            tmp = root;
        }
    }
    printf("\n");
    return putChar(res, size, '\0');
}

void debug(Node *node, FILE *file) {
    if (node == nullptr)
        return;
    
    if (node->isLeaf()) {
        fprintf(file, "%c", node->symbol);
    } else {
        debug(node->l, file);
        debug(node->r, file);
        fprintf(file, "%c", separator);
    }
}

void HuffmanTree::saveTree(FILE *file) const {
    debug(root, file);
    fprintf(file, "\n\n");
}

void traverse(Node *node, char *buffer, FILE* file, int const textLength, int &codeSum, double &entropy, int const level = 0) {
    if (node->isLeaf()) {
        buffer[level] = '\0';
        
        double probability = (double)node->frequency / textLength;
        entropy += probability * log(probability) / log(2);
        codeSum += level * node->frequency;
        
        if (node->symbol == '\n')
            fprintf(file, "\'\\n\'(ASCII code: %02X): frequency: %3d, code: %s, P(\\n) = %.9f\n", node->symbol, node->frequency, buffer, probability);
        else
            fprintf(file, "\'%c\' (ASCII code: %02X): frequency: %3d, code: %s, P(%c) = %.9f\n", node->symbol, node->symbol, node->frequency, buffer, node->symbol, probability);
        
        return;
    }
    buffer[level] = '0';
    traverse(node->l, buffer, file, textLength, codeSum, entropy, level + 1);
    buffer[level] = '1';
    traverse(node->r, buffer, file, textLength, codeSum, entropy, level + 1);
}

void HuffmanTree::saveInfo(int const codeLength, int const textLength, FILE *file) const {
    char buffer[1000];
    int codeSum = 0;
    double entropy = 0;
    fprintf(file, "Frequency table:\n");
    traverse(root, buffer, file, textLength, codeSum, entropy);
    entropy *= -1;
    
    fprintf(file, "Entropy: %.20f\n", entropy);
    fprintf(file, "Expected code length: %.20f\n", (double)codeSum / textLength);
    fprintf(file, "Length of text: %d\n", textLength * 8);
    fprintf(file, "Length of encoded text: %d\n", codeLength);
    fprintf(file, "Compression coeff.: %.20f\n", (8.0 * textLength) / codeLength);
}
