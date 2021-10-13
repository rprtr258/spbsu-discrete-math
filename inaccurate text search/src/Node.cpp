#include <string>
#include <vector>
#include <algorithm>

#include "Node.h"

using namespace std;

Node::Node(vector<string>::iterator l, vector<string>::iterator r, MetricFunction strDist) {
    data = *l;

    if (distance(l, r) == 1) {
        radius = 0;
        return;
    }
    
    radius = strDist(data, *prev(r, 1)) / 2;
    
    vector<string>::iterator i = partition(next(l), r, [&](string s){
        return strDist(s, data) <= radius;
    });
    
    if (distance(l, i) > 1)
        inner = new Node(next(l), i, strDist);
    
    if (distance(i, r) > 0)
        outer = new Node(i, r, strDist);
}

Node::~Node() {
    if (inner != nullptr)
        delete inner;
    if (outer != nullptr)
        delete outer;
}

vector<string> Node::findNearest(string str, int prec, MetricFunction strDist) {
    int d = strDist(data, str);
    vector<string> result;
    
    if (d <= prec)
       result.push_back(data);
    
    if (d + prec >= radius && outer != nullptr) {
        vector<string> add = outer->findNearest(str, prec, strDist);
        for (string s : add)
            result.push_back(s);
    }
    
    if (d - prec <= radius && inner != nullptr) {
        vector<string> add = inner->findNearest(str, prec, strDist);
        for (string s : add)
            result.push_back(s);
    }
    
    return result;
}
