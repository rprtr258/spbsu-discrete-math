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

char decodeChar(Node *root, std::vector<char unsigned> str, int unsigned &i, int unsigned &bitIndex, int unsigned &decodedBits) {
    Node *temp = root;
        
    while (!isLeaf(temp)) {
        char unsigned bit = (str[i] & (1 << (7 - bitIndex)));
        decodedBits++;
        if (bit) {
            temp = temp->r;
        } else {
            temp = temp->l;
        }
        bitIndex = bitIndex + 1;
        if (bitIndex == 8) {
            bitIndex = 0;
            i++;
        }
    }
    return temp->symbol;
}

bool isLeaf(Node *node) {
    return (node->l == nullptr && node->r == nullptr);
}
