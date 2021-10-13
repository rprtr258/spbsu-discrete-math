#include "metrics.h"
#include <vector>
#include <iostream>
#include <cctype>

// TODO: fix
int editIgnoreCaseDistance(const string& str1, const string& str2) {
    // TODO: wtf, negative indexes are used and V cannot be replaced with std::vector
    // std::vector<int> V(200);
    // static int V[200];
    int VV[400];
    int* V = VV + 200;
    int x, y;
    int offset = str1.size();
    V[offset + 1] = 0;

    for (int D = 0; D <= str1.size() + str2.size(); D++) {
        for (int k = -D; k <= D; k += 2) {
            if (k == -D || k != D && V[k - 1 + offset] < V[k + 1 + offset])
                x = V[k + 1 + offset];
            else
                x = V[k - 1 + offset] + 1;
            y = x - k;
            while (x < str1.size() && y < str2.size() && std::tolower(str1[x]) == std::tolower(str2[y])) {
                x++;
                y++;
            }
            V[k+offset] = x;
            if (x >=  str1.size() && y >= str2.size())
                return D;
        }
    }

    std::cout << "ERROR: can't find edit distance between '" << str1 << "' and '" << str2 << "'." << std::endl;
    return -1;
}

int hammingDistance(const string& str1, const string& str2) {
    int result = 0;
    int unsigned n = min(str1.length(), str2.length());
    result += max(str1.length(), str2.length()) - n;
    
    for (int unsigned i = 0; i < n; i++)
        result += (str1[i] != str2[i]);
    
    return result;
}

// TODO: fix
int editDistance(const string& str1, const string& str2) {
    // TODO: wtf, negative indexes are used and V cannot be replaced with std::vector
    // std::vector<int> V(200);
    static int V[200];
    int x, y;
    int offset = str1.size();
    V[offset + 1] = 0;

    for (int D = 0; D <= str1.size() + str2.size(); D++) {
        for (int k = -D; k <= D; k += 2) {
            if (k == -D || k != D && V[k - 1 + offset] < V[k + 1 + offset])
                x = V[k + 1 + offset];
            else
                x = V[k - 1 + offset] + 1;
            y = x - k;
            while (x < str1.size() && y < str2.size() && str1[x] == str2[y]) {
                x++;
                y++;
            }
            V[k+offset] = x;
            if (x >=  str1.size() && y >= str2.size())
                return D;
        }
    }

    std::cout << "ERROR: can't find edit distance between '" << str1 << "' and '" << str2 << "'." << std::endl;
    return -1;
}
