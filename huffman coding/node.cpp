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

char decodeChar(Node *root, std::vector<char unsigned> str, int unsigned &i) {
    Node *temp = root;
    char unsigned bit = str[i];
    while (i < str.size() && !isLeaf(temp)) {
        switch (bit) {
            case '0': {
                temp = temp->l;
                break;
            }
            case '1': {
                temp = temp->r;
                break;
            }
            default: {
                printf("Something went wrong");
                i = -1;
                break;
            }
        }
        if (isLeaf(temp))
            break;
        i++;
        bit = str[i];
    }
    return temp->symbol;
}

bool isLeaf(Node *node) {
    return (node->l == nullptr && node->r == nullptr);
}
