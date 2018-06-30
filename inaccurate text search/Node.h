#pragma once
#include <string>
#include <vector>

using namespace std;

class Node {
    private:
        Node *inner = nullptr;
        Node *outer = nullptr;
        int (*dist)(string, string) = nullptr;
        string data = nullptr;
        int radius = -1;
    public:
        Node(vector<string> list, int (*strDist)(string, string));
        string findNearest(string str);
};
