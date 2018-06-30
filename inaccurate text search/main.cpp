#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "VPTree.h"
#include "metrics.h"

using namespace std;

VPTree* constructTreeFromFile(string filename) {
    vector<string> words;
    ifstream fin(filename);
    while (!fin.eof()) {
        string newWord;
        fin >> newWord;
        if (fin.eof())
            break;
        words.push_back(newWord);
        cout << newWord << endl;
    }
    sort(words.begin(), words.end());
    return new VPTree(words, hammingDistance);
}

int main() {
    cout << "Write word to search:" << endl;
    string word = "";
    cin >> word;
    VPTree *tree = constructTreeFromFile("dict.in");
    string nearest = tree->findNearest(word);
    cout << "Nearest word is \"" << nearest << "\"" << endl;
    delete tree;
    return 0;
}
