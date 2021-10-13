#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "VPTree.h"
#include "metrics.h"

using namespace std;

int editIgnoreCaseDistance(string str1, string str2) {
    for (int unsigned i = 0; i < str1.length(); i++)
        if ('A' <= str1[i] && str1[i] <= 'Z')
            str1[i] = str1[i] - 'A' + 'a';
    
    for (int unsigned i = 0; i < str2.length(); i++)
        if ('A' <= str2[i] && str2[i] <= 'Z')
            str2[i] = str2[i] - 'A' + 'a';
    
    return editDistance(str1, str2);
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
    return new VPTree(words, editIgnoreCaseDistance);
}

vector<string> findSimilar(string str, VPTree *tree) {
    const int unsigned SIMILAR_COUNT = 5;
    int l = 0;
    int r = str.length();
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

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "Dictionary file was not provided." << endl;
        cout << "Usage: " << argv[0] << " dict.txt" << endl;
        return 1;
    }
    if (argc > 2) {
        cout << "Too many arguments." << endl;
        cout << "Usage: " << argv[0] << " dict.txt" << endl;
        return 1;
    }
    ifstream dict(argv[1]);
    if (!dict.good()) {
        cout << "Dictionary file '" << argv[1] << "' was not found." << endl;
        return 1;
    }
    cout << "Write word to search:" << endl;
    string word;
    cin >> word;
    cout << "Constructing search tree..." << endl;
    VPTree *tree = constructTreeFromFile(argv[1]);
    cout << "Tree constructed. Finding similar words..." << endl;
    vector<string> nearest = findSimilar(word, tree);
    cout << "Nearest words are:" << endl;
    for (const string& neighbour : nearest) {
        cout << neighbour << "[" << (tree->getDist())(word, neighbour) << "]" << endl;
    }
    delete tree;
    return 0;
}
