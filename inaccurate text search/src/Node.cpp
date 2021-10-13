#include <string>
#include <vector>
#include <algorithm>
#include "Node.h"

using namespace std;

Node::Node(vector<string> &list, int l, int r, int (*strDist)(string, string)) {
    dist = strDist;
    if (r == l + 1) {
        radius = 0;
        data = list[l];
        return;
    }
    
    string vantage = list[l];
    data = vantage;
    radius = dist(vantage, list[r - 1]) / 2;
    
    partition(list.begin() + l + 1, list.begin() + r, [&](string s){
        return dist(s, vantage) <= radius;
    });
    
    int i = l + 1;
    while (i < r && dist(list[i], vantage) <= radius)
        i++;
    
    if (i == l + 1)
        inner = nullptr;
    else
        inner = new Node(list, l + 1, i, strDist);
    
    if (i == r)
        outer = nullptr;
    else
        outer = new Node(list, i, r, strDist);
}

Node::~Node() {
    if (inner != nullptr)
        delete inner;
    if (outer != nullptr)
        delete outer;
}

vector<string> Node::findNearest(string str, int prec) {
    int d = dist(data, str);
    vector<string> result;
    
    if (d <= prec)
       result.push_back(data);
    
    if (d + prec >= radius && outer != nullptr) {
        vector<string> add = outer->findNearest(str, prec);
        for (string s : add)
            result.push_back(s);
    }
    
    if (d - prec <= radius && inner != nullptr) {
        vector<string> add = inner->findNearest(str, prec);
        for (string s : add)
            result.push_back(s);
    }
    
    return result;
}
