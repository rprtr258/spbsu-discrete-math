#include <string>
#include <vector>
#include <iostream>
#include "Node.h"

using namespace std;

Node::Node(vector<string> list, int (*strDist)(string, string)) {
    cout << "Proccessing: {";
    for (string s : list)
        cout << s << ", ";
    cout << "}\n";
    dist = strDist;
    if (list.size() == 1) {
        data = list[0];
        return;
    }
    int unsigned mid = list.size() / 2;
    string vantageString = list[mid];
    //for (string s : list)
    //    radius = max(radius, dist(vantageString, s));
    //vector<string> inside;
    //for (int unsigned i = 0; i < list.size(); i++) {
    //    if (i == mid)
    //        continue;
    //    string s = list[i];
    //    if (dist(vantageString, s) < radius)
    //        inside.push_back(s);
    //}
    //vector<string> outside;
    //for (int unsigned i = 0; i < list.size(); i++) {
    //    if (i == mid)
    //        continue;
    //    string s = list[i];
    //    if (dist(vantageString, s) >= radius)
    //        outside.push_back(s);
    //}
    //if (inside.size() == 0)
    //    inner = nullptr;
    //else
    //    inner = new Node(inside, strDist);
    //    
    //if (outside.size() == 0)
    //    outer = nullptr;
    //else
    //    outer = new Node(outside, strDist);
    //return;
}

string Node::findNearest(string str) {
    return "";
}
