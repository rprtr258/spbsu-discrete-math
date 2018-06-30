#include "metrics.h"

int hammingDistance(string str1, string str2) {
    int result = 0;
    int unsigned n = min(str1.length(), str2.length());
    result += max(str1.length(), str2.length()) - n;
    
    for (int unsigned i = 0; i < n; i++)
        result += (str1[i] != str2[i]);
    
    return result;
}

int editDistance(string str1, string str2) {
    return 1; // TODO: edit distance
}

int discreteDistance(string str1, string str2) {
    return (str1 != str2);
}
