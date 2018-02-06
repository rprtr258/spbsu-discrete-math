#pragma once
#include <vector>

struct Node {
    Node *l = nullptr;
    Node *r = nullptr;
    char unsigned symbol = '\0';
    int frequency = -1;
};

Node* createNode(Node *leftChild, Node *rightChild);
Node* createNode(char unsigned const symbol, int const frequency = 0);

void deleteNode(Node *&node);

char decodeChar(Node *root, std::vector<char unsigned> str, int unsigned &i);

bool isLeaf(Node *node);
