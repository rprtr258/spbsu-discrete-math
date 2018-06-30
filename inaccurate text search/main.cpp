#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "VPTree.h"
#include "metrics.h"

using namespace std;

int editAdvancedDistance(string str1, string str2) {
    int result = editDistance(str1, str2);
    
    for (int unsigned i = 0; i < str1.length(); i++)
        if ('A' <= str1[i] && str1[i] <= 'Z')
            str1[i] = str1[i] - 'A' + 'a';
    
    for (int unsigned i = 0; i < str2.length(); i++)
        if ('A' <= str2[i] && str2[i] <= 'Z')
            str2[i] = str2[i] - 'A' + 'a';
    
    result += editDistance(str1, str2) * 3;
    
    return result;
}

VPTree* constructTreeFromFile(string filename) {
    vector<string> words;
    ifstream fin(filename);
    while (!fin.eof()) {
        string newWord;
        fin >> newWord;
        if (fin.eof())
            break;
        words.push_back(newWord);
    }
    sort(words.begin(), words.end());
    return new VPTree(words, editAdvancedDistance);
}

vector<string> findSimilar(string str, VPTree *tree) {
    const int unsigned SIMILAR_COUNT = 3;
    int l = 0;
    int r = str.length() + 100500;
    while (r - l > 1) {
        int m = (l + r) / 2;
        vector<string> adj = tree->findNearest(str, m);
        if (adj.size() > SIMILAR_COUNT)
            r = m;
        else
            l = m;
    }
    vector<string> result = tree->findNearest(str, l);
    sort(result.begin(), result.end(), [&](string a, string b) {
        return (tree->getDist())(a, str) < (tree->getDist())(b, str);
    });
    return result;
}

int main() {
    cout << "Write word to search:" << endl;
    string word;
    cin >> word;
    VPTree *tree = constructTreeFromFile("dict.in");
    vector<string> nearest = findSimilar(word, tree);
    cout << "Nearest words are: {";
    for (int unsigned i = 0; i < nearest.size(); i++) {
        cout << nearest[i] << "[" << (tree->getDist())(word, nearest[i]) << "]";
        if (i < nearest.size() - 1)
            cout << ", ";
    }
    cout << "}" << endl;
    delete tree;
    return 0;
}
