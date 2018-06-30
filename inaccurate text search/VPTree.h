#pragma once
#include <string>
#include <vector>
#include "Node.h"
#include "metrics.h"

using namespace std;

class VPTree {
    private:
        Node *root = nullptr;
        int (*dist)(string, string) = discreteDistance;
    public:
        VPTree(vector<string>);
        VPTree(vector<string>, int (*)(string, string));
        ~VPTree();
        
        string findNearest(string);
};
