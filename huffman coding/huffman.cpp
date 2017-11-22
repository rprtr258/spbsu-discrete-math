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

HuffmanTree::HuffmanTree() {
}

HuffmanTree::HuffmanTree(const char *filename) {
    FrequencyTable ftable(filename);
    root = buildTree(ftable);
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

int writeCodes(Node *node, BitVector *codes, BitVector buffer = BitVector(), int const level = 0) {
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

void HuffmanTree::encode(const char *inputFile, const char *outputFile) const {
    FILE *fileInput = fopen(inputFile, "rb");
    FILE *fileOutput = fopen(outputFile, "wb");
    
    BitVector codes[alphabet];
    
    int codeLength = writeCodes(root, codes);
    
    saveTree(fileOutput);
    fprintf(fileOutput, "%d\n", codeLength);
    
    unsigned char buffer = 0;
    unsigned char bit = 7;
    int symbol ='$';
    while (symbol != EOF) {
        symbol = fgetc(fileInput);
        if (symbol == EOF)
            continue;
        for (unsigned int i = 0; i < codes[symbol].size; i++) {
            buffer = buffer | (codes[symbol].get(i) << bit);
            if (bit == 0) {
                fputc(buffer, fileOutput);
                buffer = 0;
                bit = 7;
            } else {
                bit--;
            }
        }
    }
    if (bit < 7) {
        fputc(buffer, fileOutput);
    }
    
    fclose(fileInput);
    fclose(fileOutput);
}

void HuffmanTree::decode(const char *inputFile, const char *outputFile) {
    FILE *inFile = fopen(inputFile, "rb");
    FILE *outFile = fopen(outputFile, "wb");
    
    int c = '$';
    std::stack<Node*> tmp;
    while (c != 0) {
        c = fgetc(inFile);
        if (c == 0)
            continue;
        Node* node = new Node();
        if (c != separator) {
            node->symbol = c;
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
    
    unsigned int codeLength = 0;
    while (c != '\n') {
        c = fgetc(inFile);
        if (c == '\n')
            continue;
        codeLength = codeLength * 10 + (c - '0');
    }
    
    Node *ptr = root;
    unsigned int j = 0;
    
    while (c != EOF && j < codeLength) {
        c = fgetc(inFile);
        if (c == EOF)
            continue;
        for (unsigned int i = 0; j + i < codeLength && i < 8; i++) {
            bool bit = (c  & (1 << (7 - i)));
            ptr = (!bit ? ptr->l : ptr->r);
            if (ptr->isLeaf()) {
                fputc(ptr->symbol, outFile);
                ptr = root;
            }
        }
        j += 8;
    }
    
    fclose(inFile);
    fclose(outFile);
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
