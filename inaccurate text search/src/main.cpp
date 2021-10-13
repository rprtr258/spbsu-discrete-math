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
        words.emplace_back(newWord);
    }
    return new VPTree(move(words), editIgnoreCaseDistance);
}

vector<string> findSimilar(const string& str, VPTree *tree) {
    vector<string> result = tree->findNearest(str, str.length());
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
