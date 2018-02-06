#include <stdio.h>
#include <math.h>
#include "putChar.h"
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
    //printf("\"");
    while (i < str.size() && !isLeaf(temp)) {
        //printf("%c", bit);
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
    //printf("\"\n");
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

std::vector<char unsigned> printNodeInfo(Node *node, std::vector<char unsigned> buffer, int const textLength) {
    double probability = (double)node->frequency / textLength;

    std::vector<char unsigned> result;
    result.push_back('\'');
    result.push_back(node->symbol);
    result.push_back('\'');

    //sprintf(result, "(ASCII code: %02X): frequency: %3d, code: %s", (int)node->symbol, node->frequency, buffer);

    //sprintf(result, ", P(");
    //if (node->symbol == '\n')
    //    sprintf(result, "\\n");
    //else
    //    sprintf(result, "%c", node->symbol);
    //sprintf(result, ") = %.9f\n", probability);

    return result;
}

std::vector<char unsigned> saveNodeInfo(Node *node, std::vector<char unsigned> buffer, int const textLength, int const level) {
    if (node == nullptr) {
        return std::vector<char unsigned>();
    }
    
    if (isLeaf(node)) {
        buffer[level] = '\0';
        std::vector<char unsigned> nodeInfo = printNodeInfo(node, buffer, textLength);
        return nodeInfo;
    }
    
    buffer.push_back('0');
    std::vector<char unsigned> listLeft = saveNodeInfo(node->l, buffer, textLength, level + 1);
    
    buffer.push_back('1');
    std::vector<char unsigned> listRight = saveNodeInfo(node->r, buffer, textLength, level + 1);

    std::vector<char unsigned> result;// = strConcate(listLeft, listRight);
    return result;
}
