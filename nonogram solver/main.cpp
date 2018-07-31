#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

int width, height;
vector<vector<int>> rows;
vector<vector<int>> cols;
/* -1 undefined cell
 * 0 empty cell
 * 1 filled cell
 */
vector<vector<int>> data;
vector<int> debugRow = vector<int>({3, 1, 3});

void printSolution() {
    for (int j = 0; j < width + 1 + width / 5; j++)
        cout << "\"";
    cout << endl;
    for (int i = 0; i < height; i++) {
        cout << "\"";
        for (int j = 0; j < width; j++) {
            cout << " X#"[data[i][j] + 1];
            if (j + 1 < width && (j + 1) % 5 == 0)
                cout << "|";
        }
        cout << "\"" << endl;
        if (i + 1 < height && (i + 1) % 5 == 0) {
            cout << "\"";
            for (int j = 0; j < width + width / 5 - 1; j++)
                cout << "-";
            cout << "\"" << endl;
        }
    }
    for (int j = 0; j < width + 1 + width / 5; j++)
        cout << "\"";
    cout << endl;
}

vector<int> parseString(string s) {
    vector<int> result;
    int tmp = 0;
    for (char c : s) {
        if (c == ' ') {
            result.push_back(tmp);
            tmp = 0;
        } else {
            tmp = tmp * 10 + (c - '0');
        }
    }
    result.push_back(tmp);
    return result;
}

void readTask() {
    cin >> height >> width;
    string skip;
    getline(cin, skip);
    for (int i = 0; i < height; i++) {
        string row;
        getline(cin, row);
        rows.push_back(parseString(row));
    }
    for (int i = 0; i < width; i++) {
        string col;
        getline(cin, col);
        cols.push_back(parseString(col));
    }
    data.assign(height, vector<int>(width, -1));
}

void readData() {
    int i = 0, j = 0;
    char c;
    while (i < height) {
        cin.get(c);
        if (c == ' ' || c == 'X' || c == '#') {
            if (c == ' ')
                data[i][j] = -1;
            else if (c == 'X')
                data[i][j] = 0;
            else if (c == '#')
                data[i][j] = 1;
            j++;
            if (j == width) {
                j = 0;
                i++;
            }
        }
    }
}

int firstZero(vector<int> cur, int beg, int length) {
    for (int i = 0; i < length; i++)
        if (cur[beg + i] == 0)
            return beg + i;
    return cur.size();
}

vector<int> boundsIntersectionMethod(vector<int> row, vector<int> cur) {
    vector<int> result = cur;
    for (int ptr = 0; ptr < row.size(); ptr++) {
        int l = 0;
        int r = cur.size() - 1;
        for (int i = 0; i < ptr; i++) {
            int beg = l;
            while (firstZero(cur, beg, row[i]) < cur.size())
                beg = firstZero(cur, beg, row[i]) + 1;
            l = beg + row[i] + 1;
        }
        for (int i = row.size() - 1; i > ptr; i--) {
            int end = r;
            while (firstZero(cur, end - row[i] + 1, row[i]) < cur.size())
                end--;
            r = end - row[i] - 1;
        }
        while (l < cur.size() && cur[l] == 0)
            l++;
        while (r < cur.size() && cur[r] == 0)
            r--;
        int freeSpace = r - l + 1;
        int shift = freeSpace - row[ptr];
        for (int j = l + shift; j <= r - shift; j++)
            result[j] = 1;
    }
    return result;
}

void boundsIntersectionMethod() {
    for (int i = 0; i < height; i++) {
        const vector<int> &row = rows[i];
        vector<int> curRow(width);
        for (int j = 0; j < width; j++)
            curRow[j] = data[i][j];
        vector<int> rowUpdate = boundsIntersectionMethod(row, curRow);
        for (int j = 0; j < width; j++)
            if (data[i][j] == -1)
                data[i][j] = rowUpdate[j];
    }
    for (int j = 0; j < width; j++) {
        const vector<int> &col = cols[j];
        vector<int> curCol(height);
        for (int i = 0; i < height; i++)
            curCol[i] = data[i][j];
        vector<int> colUpdate = boundsIntersectionMethod(col, curCol);
        for (int i = 0; i < height; i++)
            if (data[i][j] == -1)
                data[i][j] = colUpdate[i];
    }
}

bool check(vector<int> row, vector<int> cur) {
    vector<int> cnt;
    int x = 0;
    for (int y : cur) {
        if (y != 1 and x > 0) {
            cnt.push_back(x);
            x = 0;
        } else if (y == 1) {
            x++;
        }
    }
    if (x > 0)
        cnt.push_back(x);
    //if (row == debugRow) {
    //    for (int y : cur)
    //        cout << " X#"[y + 1];
    //    cout << ' ' << (cnt == row ? "correct" : "incorrect") << endl;
    //    for (int y : cnt)
    //        cout << y << ' ';
    //    cout << endl;
    //}
    return cnt == row;
}

vector<vector<int>> bruteforce(vector<int> row, vector<int> cur, vector<int> curState, int ptr = 0, int beg = 0) {
    int length = cur.size();
    vector<vector<int>> variants;
    for (int i = beg; i <= length - row[ptr]; i++) {
        if (firstZero(curState, i, row[ptr]) < length) {
            i = firstZero(curState, i, row[ptr]);
            continue;
        }
        vector<int> newState = curState;
        for (int j = 0; j < row[ptr]; j++)
            newState[i + j] = 1;
        if (ptr + 1 < row.size()) {
            vector<vector<int>> newVariants = bruteforce(row, cur, newState, ptr + 1, i + row[ptr] + 1);
            for (vector<int> variant : newVariants)
                variants.push_back(variant);
        } else {
            for (int &x : newState)
                if (x == -1)
                    x = 0;
            if (check(row, newState))
                variants.push_back(newState);
        }
    }
    if (ptr == 0) {
        vector<int> result = cur;
        if (variants.size() > 0)
            result = variants[0];
        else
            return vector<vector<int>>();
        for (vector<int> variant : variants) {
            for (int i = 0; i < length; i++) {
                if (result[i] != variant[i] and result[i] != -1)
                    result[i] = -1;
            }
        }
        //if (row == debugRow) {
        //    cout << "INTERSECTION" << endl;
        //    cout << "\"";
        //    for (int y : result)
        //        cout << " X#"[y + 1];
        //    cout << "\"" << endl;
        //}
        return vector<vector<int>>({result});
    }
    return variants;
}

void bruteforce() {
    for (int i = 0; i < height; i++) {
        const vector<int> &row = rows[i];
        vector<int> curRow(width);
        for (int j = 0; j < width; j++)
            curRow[j] = data[i][j];
        auto tmp = bruteforce(row, curRow, curRow);
        if (tmp.size() > 0) {
            vector<int> rowUpdate = tmp[0];
            for (int j = 0; j < width; j++)
                if (data[i][j] == -1 and rowUpdate[j] >= 0)
                    data[i][j] = rowUpdate[j];
        }
    }
    for (int j = 0; j < width; j++) {
        const vector<int> &col = cols[j];
        vector<int> curCol(height);
        for (int i = 0; i < height; i++)
            curCol[i] = data[i][j];
        auto tmp = bruteforce(col, curCol, curCol);
        if (tmp.size() > 0) {
            vector<int> colUpdate = tmp[0];
            for (int i = 0; i < height; i++)
                if (data[i][j] == -1 and colUpdate[i] >= 0)
                    data[i][j] = colUpdate[i];
        }
    }
}

bool isSolved() {
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            if (data[i][j] == -1)
                return false;
    return true;
}

int main() {
    readTask();
    //readData(); //use to start from partial solution
    while (!isSolved()) {
        boundsIntersectionMethod();
        bruteforce();
    }
    printSolution();
    return 0;
}
