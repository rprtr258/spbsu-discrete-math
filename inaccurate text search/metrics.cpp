#include "metrics.h"
#include <vector>

int hammingDistance(string str1, string str2) {
    int result = 0;
    int unsigned n = min(str1.length(), str2.length());
    result += max(str1.length(), str2.length()) - n;
    
    for (int unsigned i = 0; i < n; i++)
        result += (str1[i] != str2[i]);
    
    return result;
}

int editDistance(string str1, string str2) {
  const int unsigned m = str1.size();
  const int unsigned n = str2.size();
  if (m == 0) {
    return n;
  }
  if (n == 0) {
    return m;
  }

  std::vector< std::vector<int unsigned> > matrix(m + 1);

  for (int unsigned i = 0; i <= m; ++i) {
    matrix[i].resize(n + 1);
    matrix[i][0] = i;
  }
  for (int unsigned i = 0; i <= n; ++i) {
    matrix[0][i] = i;
  }

  int unsigned above_cell, left_cell, diagonal_cell, cost;

  for (int unsigned i = 1; i <= m; ++i) {
    for(int unsigned j = 1; j <= n; ++j) {
      cost = str1[i - 1] == str2[j - 1] ? 0 : 1;
      above_cell = matrix[i - 1][j];
      left_cell = matrix[i][j - 1];
      diagonal_cell = matrix[i - 1][j - 1];
      matrix[i][j] = std::min(std::min(above_cell + 1, left_cell + 1), diagonal_cell + cost);
    }
  }

  return matrix[m][n];
}
