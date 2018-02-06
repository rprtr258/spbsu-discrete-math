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

Node* buildTree(char const *filename) {
    std::queue<Node*> firstQueue;
    std::queue<Node*> secondQueue;
    FrequencyTable ftable(filename);
    
    for (int unsigned i = 0; i < ftable.getSize(); i++) {
        Node *leaf = createNode(ftable[i].first, ftable[i].second);
        firstQueue.push(leaf);
    }
    
    for (int unsigned i = 0; i < ftable.getSize() - 1; i++)
        proccessTwoRarest(firstQueue, secondQueue);
    
    Node *result = secondQueue.front();
    return result;
}

HuffmanTree::HuffmanTree(char const *filename) {
    root = buildTree(filename);
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

HuffmanTree::HuffmanTree(FILE *fileIn, int unsigned const treeSize) {
    std::stack<Node*> tempStack;
    
    int unsigned bitIndex = 0;
    char unsigned byte = 0;
    fscanf(fileIn, "%c", &byte);
    int unsigned length = 0;
    while (length < treeSize) {
        if (byte & (1 << (7 - bitIndex))) {
            length++;
            mergeNodes(tempStack);
        } else {
            length += 9;
            char unsigned symbol = 0;
            for (int unsigned i = 0; i < 8; i++) {
                bitIndex++;
                if (bitIndex == 8) {
                    bitIndex = 0;
                    fscanf(fileIn, "%c", &byte);
                }
                int unsigned bit = byte & (1 << (7 - bitIndex));
                symbol |= (bit << bitIndex) >> i;
            }
            pushNode(tempStack, symbol);
        }
        bitIndex++;
        if (bitIndex == 8) {
            bitIndex = 0;
            fscanf(fileIn, "%c", &byte);
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

void HuffmanTree::encode(char const *filename, FILE *outFile) {
    char unsigned *codes[alphabet];
    char unsigned buffer[alphabet];
    for (int unsigned i = 0; i < alphabet; i++) {
        codes[i] = nullptr;
        buffer[i] = '\0';
    }
    
    writeCodes(root, codes, buffer);
    
    int unsigned bitIndex = 0;
    FILE *file = fopen(filename, "rb");
    char unsigned outByte = 0;
    bool firstWrite = true;
    while (!feof(file)) {
        char unsigned byte = 0;
        fscanf(file, "%c", &byte);
        if (feof(file))
            break;
        int unsigned const charCode = (int unsigned)byte;
        int unsigned codeLength = strlen(codes[charCode]);
        for (int unsigned k = 0; k < codeLength; k++) {
            if (bitIndex == 0) {
                if (firstWrite)
                    firstWrite = false;
                else
                    fprintf(outFile, "%c", outByte);
                outByte = 0;
            }
            int unsigned bit = (codes[charCode][k] == '1');
            outByte |= (bit << (7 - bitIndex));
            bitIndex = (bitIndex + 1) % 8;
        }
    }
    if (bitIndex != 0)
        fprintf(outFile, "%c", outByte);
    fclose(file);

    for (int unsigned i = 0; i < alphabet; i++)
        if (codes[i] != nullptr)
            delete[] codes[i];
}

void HuffmanTree::decode(FILE *fileIn, char const *filename, int unsigned const length) {
    int unsigned bitIndex = 0;
    int unsigned i = 0;
    int unsigned decodedBits = 0;
    char unsigned byte = 0;
    fscanf(fileIn, "%c", &byte);
    FILE *fileOut = fopen(filename, "wb");
    while (decodedBits < length) {
        char unsigned newSymbol = decodeChar(root, fileIn, i, byte, bitIndex, decodedBits);
        fprintf(fileOut, "%c", newSymbol);
    }
    fclose(fileOut);
}

void saveNode(Node *node, ByteString &res, int unsigned &bitIndex, int unsigned &length) {
    if (node == nullptr) {
        return;
    }
    
    if (isLeaf(node)) {
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

int unsigned HuffmanTree::getResultLength() {
    return calcResultLength(root);
}

ByteString HuffmanTree::asString(int unsigned &length) {
    ByteString result;
    int unsigned bitIndex = 0;
    saveNode(root, result, bitIndex, length);
    return result;
}

