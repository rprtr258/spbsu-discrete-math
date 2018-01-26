#pragma once

struct Node {
    Node *l = nullptr;
    Node *r = nullptr;
    char symbol = '\0';
    int frequency = -1;
};

Node* createNode(Node *leftChild, Node *rightChild);
Node* createNode(char const symbol, int const frequency = 0);
Node* copy(Node *node);

void deleteNode(Node *&node);

char decodeChar(Node *root, FILE *file, char const firstBit);

bool isLeaf(Node *node);
int getHeight(Node *node);
int getCodeLength(Node *node, int const level = 0);
double getEntropy(Node *node, int const textLength);

void saveNodeInfo(Node *node, char *buffer, FILE* file, int const textLength, int const level = 0);
