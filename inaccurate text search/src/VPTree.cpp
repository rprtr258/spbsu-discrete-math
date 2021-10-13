#include "VPTree.h"

VPTree::VPTree(vector<string>&& list, MetricFunction strDist) {
    dist = strDist;
    root = new Node(list.begin(), list.end(), strDist);
    return;
}

VPTree::~VPTree() {
    delete root;
}

vector<string> VPTree::findNearest(string str, int prec) {
    return root->findNearest(str, prec, dist);
}

MetricFunction VPTree::getDist() {
    return dist;
}
