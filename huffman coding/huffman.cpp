#include <string.h>
#include <stdio.h>
#include <queue>
#include <stack>
#include "huffman.h"
#include "node.h"
#include "freqtable.h"
#include "putChar.h"

int unsigned const alphabet = 256;
char const separator = '\7';

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
            result = firstQueue.front();
            firstQueue.pop();
	} else {
	    result = secondQueue.front();
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

Node* buildTree(char const *str) {
    std::queue<Node*> firstQueue;
    std::queue<Node*> secondQueue;
    
    FrequencyTable ftable(str);
    
    for (int unsigned i = 0; i < ftable.getSize(); i++) {
        Node *leaf = createNode(ftable.data[i].first, ftable.data[i].second);
        firstQueue.push(leaf);
    }
    
    for (int unsigned i = 0; i < ftable.getSize() - 1; i++)
        proccessTwoRarest(firstQueue, secondQueue);
    
    Node *result = secondQueue.front();
    
    return result;
}

HuffmanTree* createTree(const char *str) {
    HuffmanTree *tree = new HuffmanTree();
    tree->root = buildTree(str);
    return tree;
}

void proccesSymbol(std::stack<Node*> stack, char const symbol) {
    Node *node = nullptr;
    if (symbol == separator) {
        Node *rightChild = stack.top();
	stack.pop();
        Node *leftChild = stack.top();
	stack.pop();

        node = createNode(leftChild, rightChild);

        deleteNode(leftChild);
        deleteNode(rightChild);
    } else {
        node = createNode(symbol == '\0' ? '\n' : symbol);
    }
    stack.push(node);
}

HuffmanTree* readTree(const char *filename) {
    std::stack<Node*> tempStack;
    FILE *file = fopen(filename, "r");
    
    char symbol = '\0';
    fscanf(file, "%c", &symbol);
    while (symbol != '\n') {
        proccesSymbol(tempStack, symbol);
        fscanf(file, "%c", &symbol);
    }
    
    HuffmanTree *result = new HuffmanTree();
    result->root = tempStack.top();
    
    fclose(file);
    return result;
}

void deleteTree(HuffmanTree *&tree) {
    if (tree == nullptr)
        return;
    deleteNode(tree->root);
    delete tree;
    tree = nullptr;
}

void writeCodes(Node *node, char **codes, char *buffer, int const level = 0) {
    if (isLeaf(node)) {
        int unsigned const symbolCode = (int)node->symbol;
        codes[symbolCode] = new char[level + 1];
        memcpy(codes[symbolCode], buffer, level * sizeof(char));
        codes[symbolCode][level] = '\0';
        return;
    }
    buffer[level] = '0';
    writeCodes(node->l, codes, buffer, level + 1);
    buffer[level] = '1';
    writeCodes(node->r, codes, buffer, level + 1);
}

char* encode(HuffmanTree *tree, const char *str) {
    char *codes[alphabet];
    for (int i = 0; i < alphabet; i++)
        codes[i] = nullptr;
    
    char buffer[alphabet];
    writeCodes(tree->root, codes, buffer);
    
    int strLength = strlen(str);
    int resultLength = getCodeLength(tree->root);
    char *result = new char[resultLength + 1];
    int j = 0;
    for (int i = 0; i < strLength; i++) {
        int unsigned const symbolCode = (int)str[i];
        int codeLength = strlen(codes[symbolCode]);
        memcpy(result + j, codes[symbolCode], codeLength * sizeof(char));
        j += codeLength;
    }
    result[resultLength] = '\0';
    
    for (int i = 0; i < alphabet; i++)
        if (codes[i] != nullptr)
            delete[] codes[i];
    return result;
}

char* decodeFile(HuffmanTree *tree, const char *fileInput) {
    char symbol = '\0';
    FILE *file = fopen(fileInput, "r");
    
    while (symbol != '\n')
        fscanf(file, "%c", &symbol);
    
    char *result = new char[1];
    result[0] = '\0';
    fscanf(file, "%c", &symbol);
    while (!feof(file)) {
        char newSymbol = decodeChar(tree->root, file, symbol);
        putChar(result, newSymbol);
        
        fscanf(file, "%c", &symbol);
    }
    
    fclose(file);
    return result;
}

void saveNode(Node *node, FILE *file) {
    if (isLeaf(node)) {
        // WARNING: \n encoded as \0 so there is only one line for tree
        fprintf(file, "%c", (node->symbol == '\n' ? '\0' : node->symbol));
    } else {
        saveNode(node->l, file);
        saveNode(node->r, file);
        fprintf(file, "%c", separator);
    }
}

void saveTree(HuffmanTree *tree, FILE *file) {
    saveNode(tree->root, file);
    fprintf(file, "\n");
}

void saveInfo(HuffmanTree *tree, FILE *file, int const textLength) {
    char *buffer = new char[alphabet];
    double entropy = getEntropy(tree->root, textLength);
    fprintf(file, "Frequency table:\n");
    int codeLength = getCodeLength(tree->root);
    saveNodeInfo(tree->root, buffer, file, textLength);
    
    fprintf(file, "Entropy: %.20f\n", entropy);
    fprintf(file, "Expected code length: %.20f\n", (double)codeLength / textLength);
    fprintf(file, "Length of text(in bytes): %d\n", textLength);
    fprintf(file, "Length of encoded text(in bytes): %d\n", codeLength / 8);
    fprintf(file, "Compression coeff.: %.20f\n", textLength / (codeLength / 8.0));
    
    delete[] buffer;
}
