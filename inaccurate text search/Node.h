#pragma once
#include <string>
#include <vector>

using namespace std;

class Node {
    private:
        Node *inner = nullptr;
        Node *outer = nullptr;
        int (*dist)(string, string) = nullptr;
        string data = "";
        int radius = -1;
    public:
        Node(vector<string> &list, int l, int r, int (*strDist)(string, string));
        ~Node();
        
        vector<string> findNearest(string str, int prec);
};
