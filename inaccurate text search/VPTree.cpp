#include "VPTree.h"

VPTree::VPTree(vector<string> list) {
    root = new Node(list, dist);
}

VPTree::VPTree(vector<string> list, int (*strDist)(string, string)) {
    dist = strDist;
    //root = new Node(list, strDist);
    return;
}

VPTree::~VPTree() {
    delete root;
}

string VPTree::findNearest(string str) {
    return root->findNearest(str);
}
