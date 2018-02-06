#include <stdio.h>
#include <math.h>
#include "node.h"

Node* createNode(Node *leftChild, Node *rightChild) {
    Node *result = new Node();
    result->l = leftChild;
    result->r = rightChild;
    result->frequency = leftChild->frequency + rightChild->frequency;
    return result;
}

Node* createNode(char unsigned const symbol, int const frequency) {
    Node *result = new Node();
    result->symbol = symbol;
    result->frequency = frequency;
    return result;
}

void deleteNode(Node *&node) {
    if (node == nullptr)
        return;
    delete node;
}

char decodeChar(Node *root, FILE *file, int unsigned &i, char unsigned &byte, int unsigned &bitIndex, int unsigned &decodedBits) {
    Node *temp = root;
        
    while (!isLeaf(temp)) {
        char unsigned bit = (byte & (1 << (7 - bitIndex)));
        decodedBits++;
        if (bit == 0)
            temp = temp->l;
        else
            temp = temp->r;

        bitIndex = bitIndex + 1;
        if (bitIndex == 8) {
            bitIndex = 0;
            i++;
            fscanf(file, "%c", &byte);
        }
    }
    return temp->symbol;
}

int unsigned calcResultLength(Node *node, int unsigned const level) {
    if (isLeaf(node))
        return node->frequency * level;
    return calcResultLength(node->l, level + 1) +
           calcResultLength(node->r, level + 1);
}

bool isLeaf(Node *node) {
    return (node->l == nullptr && node->r == nullptr);
}
