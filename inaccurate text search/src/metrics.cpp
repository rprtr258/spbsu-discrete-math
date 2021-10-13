#include "metrics.h"
#include <vector>
#include <iostream>

int editIgnoreCaseDistance(const string& str1, const string& str2) {
    string str1Copy = str1;
    string str2Copy = str2;
    for (char &c : str1Copy)
        if ('A' <= c && c <= 'Z')
            c = c - 'A' + 'a';
    for (char &c : str2Copy)
        if ('A' <= c && c <= 'Z')
            c = c - 'A' + 'a';
    return editDistance(str1Copy, str2Copy);
}

int hammingDistance(const string& str1, const string& str2) {
    int result = 0;
    int unsigned n = min(str1.length(), str2.length());
    result += max(str1.length(), str2.length()) - n;
    
    for (int unsigned i = 0; i < n; i++)
        result += (str1[i] != str2[i]);
    
    return result;
}

int editDistance(const string& str1, const string& str2) {
    const int unsigned m = str1.size();
    const int unsigned n = str2.size();
    if (m == 0) {
        return n;
    }
    if (n == 0) {
        return m;
    }
    static int V[200];
    int x, y;
    int offset = str1.size();
    V[offset + 1] = 0;

    for (int D = 0; D <= str1.size() + str2.size(); D++) {
        for (int k = -D; k <= D; k += 2) {
            if (k == -D || k != D && V[k-1+offset] < V[k+1+offset])
                x = V[k+1+offset];
            else
                x = V[k-1+offset] + 1;
            y = x - k;
            while (x < str1.size() && y < str2.size() && str1[x] == str2[y]) {
                x++;
                y++;
            }
            V[k+offset] = x;
            if (x >=  str1.size() && y >= str2.size()) return D;
        }
    }

    std::cout << "WTF" << std::endl;
    return -1;
}
