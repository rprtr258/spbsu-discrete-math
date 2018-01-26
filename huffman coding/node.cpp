#include <stdio.h>
#include <math.h>
#include "huffmanNode.h"

Node* createNode(Node *leftChild, Node *rightChild) {
    Node *result = new Node();
    result->l = copy(leftChild);
    result->r = copy(rightChild);
    result->frequency = leftChild->frequency + rightChild->frequency;
    return result;
}

Node* createNode(char const symbol, int const frequency) {
    Node *result = new Node();
    result->symbol = symbol;
    result->frequency = frequency;
    return result;
}

Node* copy(Node *node) {
    if (node == nullptr)
        return nullptr;
    
    Node *result = new Node();
    result->l = copy(node->l);
    result->r = copy(node->r);
    result->symbol = node->symbol;
    result->frequency = node->frequency;
    
    return result;
}

void deleteNode(Node *&node) {
    if (node == nullptr)
        return;
    deleteNode(node->l);
    deleteNode(node->r);
    delete node;
}

char decodeChar(Node *root, FILE *file, char const firstBit) {
    Node *temp = root;
    char bit = firstBit;
    while (!isLeaf(temp)) {
        if (bit == '0')
            temp = temp->l;
        else
            temp = temp->r;
        if (isLeaf(temp))
            break;
        fscanf(file, "%c", &bit);
    }
    return temp->symbol;
}

bool isLeaf(Node *node) {
    return (node->l == nullptr && node->r == nullptr);
}

int getHeight(Node *node) {
    if (isLeaf(node))
        return 0;
    int heightLeft = getHeight(node->l);
    int heightRight = getHeight(node->r);
    return (heightLeft >= heightRight ? heightLeft : heightRight) + 1;
}

int getCodeLength(Node *node, int const level) {
    if (node == nullptr)
        return 0;
    
    if (isLeaf(node))
        return node->frequency * level;
    
    return getCodeLength(node->l, level + 1) + getCodeLength(node->r, level + 1);
}

double getEntropy(Node *node, int const textLength) {
    if (node == nullptr)
        return -1;
    
    if (isLeaf(node)) {
        double probability = (double)node->frequency / textLength;
        return probability * log(1.0 / probability) / log(2);
    }
    
    return getEntropy(node->l, textLength) + getEntropy(node->r, textLength);
}

void printEncodedChar(FILE *file, char const symbol) {
    if (symbol == '\n')
        fprintf(file, "\\n");
    else
        fprintf(file, "%c", symbol);
}

void printNodeInfo(Node *node, char *buffer, FILE* file, int const textLength) {
    double probability = (double)node->frequency / textLength;

    fprintf(file, "\'");
    printEncodedChar(file, node->symbol);
    fprintf(file, "\'");

    fprintf(file, "(ASCII code: %02X): frequency: %3d, code: %s", (int)node->symbol, node->frequency, buffer);

    fprintf(file, ", P(");
    printEncodedChar(file, node->symbol);
    fprintf(file, ") = %.9f\n", probability);
}

void saveNodeInfo(Node *node, char *buffer, FILE* file, int const textLength, int const level) {
    if (node == nullptr)
        return;
    
    if (isLeaf(node)) {
        buffer[level] = '\0';
        printNodeInfo(node, buffer, file, textLength);
        return;
    }
    
    buffer[level] = '0';
    saveNodeInfo(node->l, buffer, file, textLength, level + 1);
    
    buffer[level] = '1';
    saveNodeInfo(node->r, buffer, file, textLength, level + 1);
}
