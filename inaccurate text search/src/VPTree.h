#pragma once
#include <string>
#include <vector>
#include "Node.h"
#include "metrics.h"

using namespace std;

typedef int (*MetricFunction)(string, string);

class VPTree {
    private:
        Node *root = nullptr;
        MetricFunction dist = nullptr;
    public:
        VPTree(vector<string>&, MetricFunction);
        ~VPTree();
        
        vector<string> findNearest(string, int);
        MetricFunction getDist();
};
