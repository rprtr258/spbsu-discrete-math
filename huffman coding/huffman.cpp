#include <string.h>
#include <stdio.h>
#include <queue>
#include <vector>
#include <stack>
#include "node.h"
#include "freqtable.h"
#include "huffman.h"

int unsigned const alphabet = 256;

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

void pushNode(std::stack<Node*> &stack, char const symbol) {
    Node *node = createNode(symbol);
    stack.push(node);
}

void mergeNodes(std::stack<Node*> &stack) {
    Node *rightChild = stack.top();
    stack.pop();
    Node *leftChild = stack.top();
    stack.pop();

    Node *node = createNode(leftChild, rightChild);
    stack.push(node);
}

HuffmanTree::HuffmanTree(ByteString tree, int unsigned const treeSize) {
    std::stack<Node*> tempStack;
    
    int unsigned bitIndex = 0;
    int unsigned byteIndex = 0;
    int unsigned length = 0;
    while (length < treeSize) {
        if (tree[byteIndex] & (1 << (7 - bitIndex))) {
            length++;
            mergeNodes(tempStack);
            bitIndex++;
            if (bitIndex == 8) {
                bitIndex = 0;
                byteIndex++;
            }
        } else {
            length += 9;
            bitIndex++;
            if (bitIndex == 8) {
                bitIndex = 0;
                byteIndex++;
            }
            char unsigned symbol = 0;
            for (int unsigned i = 0; i < 8; i++) {
                int unsigned bit = tree[byteIndex] & (1 << (7 - bitIndex));
                symbol |= (bit << bitIndex) >> i;
                bitIndex++;
                if (bitIndex == 8) {
                    bitIndex = 0;
                    byteIndex++;
                }
            }
            pushNode(tempStack, symbol);
        }
    }
    
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

void saveNode(Node *node, ByteString &res, int unsigned &bitIndex, int unsigned &length) {
    if (node == nullptr) {
        return;
    } else if (isLeaf(node)) {
        char unsigned byte = node->symbol;
        length += 9;
        if (bitIndex == 0)
            res.push_back(0);
        bitIndex = (bitIndex + 1) % 8;
        for (int unsigned i = 0; i < 8; i++) {
            if (bitIndex == 0)
                res.push_back(0);
            int unsigned bit = (byte & (1 << (7 - i)));
            res.back() |= ((bit << i) >> bitIndex);
            
            bitIndex = (bitIndex + 1) % 8;
        }
    } else {
        length++;
        saveNode(node->l, res, bitIndex, length);
        saveNode(node->r, res, bitIndex, length);
        if (bitIndex == 0)
            res.push_back(0);
        res.back() |= (1 << (7 - bitIndex));
        bitIndex = (bitIndex + 1) % 8;
    }
}

ByteString HuffmanTree::asString(int unsigned &length) {
    ByteString result;
    int unsigned bitIndex = 0;
    saveNode(root, result, bitIndex, length);
    return result;
}

