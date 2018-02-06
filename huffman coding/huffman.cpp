#include <string.h>
#include <stdio.h>
#include <queue>
#include <vector>
#include <stack>
#include "node.h"
#include "freqtable.h"
#include "huffman.h"

int unsigned const alphabet = 256;
char const SEPARATOR = '\7';

int unsigned strlen(char unsigned const *str) {
    int unsigned result = 0;
    while (str[result] != '\0')
        result++;
    return result;
}

Node* getRarestNode(std::queue<Node*> &firstQueue, std::queue<Node*> &secondQueue) {
    Node *result = nullptr;
    if (firstQueue.size() == 0) {
        result = secondQueue.front();
        secondQueue.pop();
    } else if (secondQueue.size() == 0) {
        result = firstQueue.front();
        firstQueue.pop();
    } else {
        Node *tempFirst = firstQueue.front();
        Node *tempSecond = secondQueue.front();
        
        if (tempFirst->frequency < tempSecond->frequency) {
            result = tempFirst;
            firstQueue.pop();
        } else {
            result = tempSecond;
            secondQueue.pop();
        }
    }
    return result;
};

void proccessTwoRarest(std::queue<Node*> &firstQueue, std::queue<Node*> &secondQueue) {
    Node *first = getRarestNode(firstQueue, secondQueue);
    Node *second = getRarestNode(firstQueue, secondQueue);
    
    Node *parent = createNode(first, second);
    secondQueue.push(parent);
}

Node* buildTree(ByteString str) {
    std::queue<Node*> firstQueue;
    std::queue<Node*> secondQueue;
    FrequencyTable ftable(str);
    
    for (int unsigned i = 0; i < ftable.getSize(); i++) {
        Node *leaf = createNode(ftable[i].first, ftable[i].second);
        firstQueue.push(leaf);
    }
    
    for (int unsigned i = 0; i < ftable.getSize() - 1; i++)
        proccessTwoRarest(firstQueue, secondQueue);
    
    Node *result = secondQueue.front();
    return result;
}

HuffmanTree::HuffmanTree(ByteString str) {
    root = buildTree(str);
}

void proccesSymbol(std::stack<Node*> &stack, char const symbol) {
    Node *node = nullptr;
    if (symbol == SEPARATOR) {
        Node *rightChild = stack.top();
        stack.pop();
        Node *leftChild = stack.top();
        stack.pop();

        node = createNode(leftChild, rightChild);
    } else {
        node = createNode(symbol);
    }
    stack.push(node);
}

HuffmanTree::HuffmanTree(ByteString tree, int unsigned const treeSize) {
    std::stack<Node*> tempStack;
    
    for (int unsigned i = 0; i < treeSize; i++)
        proccesSymbol(tempStack, tree[i + 3]);
    
    root = tempStack.top();
}

HuffmanTree::~HuffmanTree() {
    if (root == nullptr)
        return;
    deleteNode(root);
}

void writeCodes(Node *node, char unsigned *codes[alphabet], char unsigned buffer[alphabet], int const level = 0) {
    if (isLeaf(node)) {
        int unsigned const symbolCode = (int unsigned)node->symbol;
        codes[symbolCode] = new char unsigned[level + 1];
        memcpy(codes[symbolCode], buffer, level * sizeof(char unsigned));
        codes[symbolCode][level] = '\0';
        return;
    }
    buffer[level] = '0';
    writeCodes(node->l, codes, buffer, level + 1);
    buffer[level] = '1';
    writeCodes(node->r, codes, buffer, level + 1);
}

ByteString HuffmanTree::encode(ByteString str) {
    char unsigned *codes[alphabet];
    char unsigned buffer[alphabet];
    for (int unsigned i = 0; i < alphabet; i++) {
        codes[i] = nullptr;
        buffer[i] = '\0';
    }
    
    writeCodes(root, codes, buffer);
    
    ByteString result;
    int j = 0;
    for (int unsigned i = 0; i < str.size(); i++) {
        int unsigned const symbolCode = (int)str[i];
        int unsigned codeLength = strlen(codes[symbolCode]);
        for (int unsigned k = 0; k < codeLength; k++)
            result.push_back(codes[symbolCode][k]);
        j += codeLength;
    }

    for (int unsigned i = 0; i < alphabet; i++)
        if (codes[i] != nullptr)
            delete[] codes[i];

    return result;
}

ByteString HuffmanTree::decode(ByteString str, int unsigned const length) {
    ByteString result;
    for (int unsigned i = 0; i < length; i++) {
        char unsigned newSymbol = decodeChar(root, str, i);
        result.push_back(newSymbol);
    }
    return result;
}

ByteString saveNode(Node *node) {
    ByteString result;
    if (isLeaf(node)) {
        result.push_back(node->symbol);
    } else {
        result = saveNode(node->l);
        ByteString second = saveNode(node->r);
	for (const char unsigned &byte : second)
	    result.push_back(byte);
        result.push_back(SEPARATOR);
    }
    return result;
}

ByteString HuffmanTree::asString() {
    ByteString result = saveNode(root);
    return result;
}

