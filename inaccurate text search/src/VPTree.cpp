#include <algorithm>
#include <iostream>

#include "VPTree.h"

VPTree::VPTree(vector<string>&& list, MetricFunction strDist) :
    dist(strDist), data(list), outer(list.size(), list.size()), radius(list.size(), 0), has_inner(list.size(), false) {
    init_self(list.begin(), list.end(), 0);
}

void VPTree::init_self(vector<string>::iterator l, vector<string>::iterator r, const int node) {
    if (distance(l, r) == 1) {
        return;
    }
    
    radius[node] = dist(*l, *prev(r, 1)) / 2;
    
    vector<string>::iterator i = partition(next(l), r, [&](string s){
        return dist(s, *l) <= radius[node];
    });
    const int node_outer = node + std::distance(l, i);
    if (distance(l, i) > 1) {
        has_inner[node] = true;
        init_self(next(l), i, node + 1);
    }
    
    if (distance(i, r) > 0) {
        outer[node] = node_outer;
        init_self(i, r, node_outer);
    }
}

vector<string> VPTree::_findNearest(const string &str, int prec, const int node) const {
    int d = dist(data[node], str);
    vector<string> result;
    
    if (d <= prec)
       result.push_back(data[node]);
    
    if (d - prec <= radius[node] && has_inner[node]) {
        vector<string> add = _findNearest(str, prec, node + 1);
        for (const string& x : add)
            result.emplace_back(x);
    }
    
    if (d + prec >= radius[node] && (data.size() - outer[node]) != 0) {
        vector<string> add = _findNearest(str, prec, outer[node]);
        for (const string& x : add)
            result.emplace_back(x);
    }
    
    return result;
}

vector<string> VPTree::findNearest(const string& str, int prec) const {
    return _findNearest(str, prec, 0);
}

int VPTree::_countNearest(const string &str, int prec, const int node) const {
    int d = dist(data[node], str);
    int result = 0;
    
    if (d <= prec)
       result++;
    
    if (d - prec <= radius[node] && has_inner[node]) {
        result += _countNearest(str, prec, node + 1);
    }
    
    if (d + prec >= radius[node] && (data.size() - outer[node]) != 0) {
        result += _countNearest(str, prec, outer[node]);
    }
    
    return result;
}

int VPTree::countNearest(const string& str, int prec) const {
    return _countNearest(str, prec, 0);
}

MetricFunction VPTree::getDist() {
    return dist;
}
