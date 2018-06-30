#include <string>
#include <vector>
#include <iostream>
#include "Node.h"

using namespace std;

Node::Node(vector<string> list, int (*strDist)(string, string)) {
    dist = strDist;
    if (list.size() == 1) {
        radius = 0;
        data = list[0];
        return;
    }
    
    int unsigned mid = list.size() / 2;
    string vantage = list[mid];
    data = vantage;
    for (string s : list)
        radius = max(radius, dist(vantage, s));
    //radius = radius / 2;
    radius = min(3, radius);
    
    vector<string> inside;
    vector<string> outside;
    for (string s : list)
        if (dist(vantage, s) == 0)
            continue;
        else if (dist(vantage, s) <= radius)
            inside.push_back(s);
        else
            outside.push_back(s);
    
    if (inside.size() == 0)
        inner = nullptr;
    else
        inner = new Node(inside, strDist);
    
    if (outside.size() == 0)
        outer = nullptr;
    else
        outer = new Node(outside, strDist);
}

Node::~Node() {
    if (inner != nullptr)
        delete inner;
    if (outer != nullptr)
        delete outer;
}

vector<string> Node::findNearest(string str, int prec) {
    //cout << "(" << data << ", " << radius << ")" << endl;
    //cout << "d(" << str << ", " << data << ") = " << dist(data, str) << endl;
    
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
