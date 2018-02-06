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
int getHeight(Node *node);
int getCodeLength(Node *node, int const level = 0);
double getEntropy(Node *node, int const textLength);

std::vector<char unsigned> saveNodeInfo(Node *node, std::vector<char unsigned> buffer, int const textLength, int const level = 0);
