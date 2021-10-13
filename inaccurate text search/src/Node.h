#pragma once
#include <string>
#include <vector>

using namespace std;

typedef int (*MetricFunction)(const string&, const string&);

class Node {
    private:
        Node *inner = nullptr;
        Node *outer = nullptr;
        MetricFunction dist = nullptr;
        string data = "";
        int radius = -1;
    public:
        Node(vector<string>::iterator l, vector<string>::iterator r, MetricFunction strDist);
        ~Node();
        
        vector<string> findNearest(string str, int prec);
};
