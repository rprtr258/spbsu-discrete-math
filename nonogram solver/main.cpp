#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <cstring>

using namespace std;

int width, height;
vector<vector<int>> rows;
vector<vector<int>> cols;
vector<vector<vector<int>>> rowsVariants;
vector<vector<vector<int>>> colsVariants;
bool doVerbose = false;
/* -1 undefined cell
 * 0 empty cell
 * 1 filled cell
 */
vector<vector<int>> data;

vector<int> getRow(int i) {
    vector<int> result(width);
    for (int j = 0; j < width; j++)
        result[j] = data[i][j];
    return result;
}

vector<int> getCol(int j) {
    vector<int> result(height);
    for (int i = 0; i < height; i++)
        result[i] = data[i][j];
    return result;
}

void updateRow(int i, vector<int> rowUpdate) {
    for (int j = 0; j < width; j++)
        if (data[i][j] == -1)
            data[i][j] = rowUpdate[j];
}

void updateCol(int j, vector<int> colUpdate) {
    for (int i = 0; i < height; i++)
        if (data[i][j] == -1)
            data[i][j] = colUpdate[i];
}

void printSolution(const bool &doFormat) {
    if (doFormat) {
        for (int j = 0; j < width + 1 + width / 5; j++)
            cout << "\"";
        cout << endl;
    }
    for (int i = 0; i < height; i++) {
        if (doFormat) {
            cout << "\"";
        }
        for (int j = 0; j < width; j++) {
            cout << "  #"[data[i][j] + 1];
            if (doFormat) {
                if (j + 1 < width && (j + 1) % 5 == 0)
                    cout << "|";
            }
        }
        if (doFormat) {
            cout << "\"" << endl;
            if (i + 1 < height && (i + 1) % 5 == 0) {
                cout << "\"";
                for (int j = 0; j < width + width / 5 - 1; j++)
                    cout << "-";
                cout << "\"" << endl;
            }
        } else {
            cout << endl;
        }
    }
    if (doFormat) {
        for (int j = 0; j < width + 1 + width / 5; j++)
            cout << "\"";
        cout << endl;
    }
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
            //while (beg + row[ptr] < cur.size() && cur[beg + row[ptr]] == 1)
            //    beg++;
            l = beg + row[i] + 1;
        }
        for (int i = row.size() - 1; i > ptr; i--) {
            int end = r;
            while (firstZero(cur, end - row[i] + 1, row[i]) < cur.size())
                end--;
            // same
            r = end - row[i] - 1;
        }
        while (l < cur.size() && cur[l] == 0)
            l++;
        while (r < cur.size() && cur[r] == 0)
            r--;
        int freeSpace = r - l + 1;
        int shift = freeSpace - row[ptr];
        if (l + shift <= r - shift)
            fill(result.begin() + l + shift, result.begin() + r - shift + 1, 1);
    }
    return result;
}

void boundsIntersectionMethod() {
    for (int i = 0; i < height; i++) {
        vector<int> rowUpdate = boundsIntersectionMethod(rows[i], getRow(i));
        updateRow(i, rowUpdate);
    }
    for (int j = 0; j < width; j++) {
        vector<int> colUpdate = boundsIntersectionMethod(cols[j], getCol(j));
        updateCol(j, colUpdate);
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
    return (cnt == row);
}

vector<int> intersectVariants(vector<vector<int>> &v) {
    assert(v.size() > 0);
    vector<int> result = v[0];
    for (vector<int> variant : v) {
        for (int i = 0; i < result.size(); i++) {
            if (result[i] != variant[i])
                result[i] = -1;
        }
    }
    return result;
}

vector<vector<int>> bruteforce(vector<int> row, vector<int> state, int ptr = 0, int beg = 0) {
    int length = state.size();
    vector<vector<int>> variants;
    for (int i = beg; i <= length - row[ptr]; i++) {
        if (firstZero(state, i, row[ptr]) < length) {
            i = firstZero(state, i, row[ptr]);
            continue;
        }
        //while (state[i + row[ptr]] == 1)
        //    i++;
        vector<int> newState = state;
        fill(newState.begin() + i, newState.begin() + i + row[ptr], 1);
        if (ptr + 1 < row.size()) {
            vector<vector<int>> newVariants = bruteforce(row, newState, ptr + 1, i + row[ptr] + 1);
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
        variants.push_back(intersectVariants(variants));
        return variants;
    }
    return variants;
}

void bruteforce() {
    for (int i = 0; i < height; i++) {
        auto tmp = bruteforce(rows[i], getRow(i));
        updateRow(i, tmp.back());
        tmp.pop_back();
        rowsVariants.push_back(tmp);
    }
    for (int j = 0; j < width; j++) {
        auto tmp = bruteforce(cols[j], getCol(j));
        updateCol(j, tmp.back());
        tmp.pop_back();
        colsVariants.push_back(tmp);
    }
}

vector<int> bruteforceSelect(vector<int> row, vector<int> cur, vector<vector<int>> &variants) {
    vector<vector<int>> newVariants;
    for (vector<int> variant : variants) {
        bool valid = true;
        vector<int> modifiedVariant = variant;
        for (int i = 0; i < cur.size(); i++)
            if (cur[i] != -1 && modifiedVariant[i] != cur[i])
                valid = false;
        valid &= check(row, modifiedVariant);
        if (valid)
            newVariants.push_back(modifiedVariant);
    }
    variants = newVariants;
    vector<int> result = intersectVariants(variants);
    return result;
}

void bruteforceSelect() {
    for (int i = 0; i < height; i++) {
        vector<int> rowUpdate = bruteforceSelect(rows[i], getRow(i), rowsVariants[i]);
        updateRow(i, rowUpdate);
    }
    for (int j = 0; j < width; j++) {
        vector<int> colUpdate = bruteforceSelect(cols[j], getCol(j), colsVariants[j]);
        updateCol(j, colUpdate);
    }
}

int getProgress() {
    int result = 0;
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            result += (data[i][j] != -1);
    return result;
}

void printProgress() {
    if (doVerbose) {
        int progress = getProgress();
        cout << progress << "/" << width * height << endl;
        cout.flush();
    }
}

bool isSolved() {
    return (getProgress() == width * height);
}

void runTillStabilize(auto getStatus, void (*proccess)()) {
    int curStatus = getStatus();
    printProgress();
    while (true) {
        proccess();
        int newStatus = getStatus();
        if (newStatus != curStatus) {
            curStatus = newStatus;
            printProgress();
        } else {
            break;
        }
    }
}

int main(int argc, char ** argv) {
    bool doReadPartial = false;
    bool doFormat = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp("--help", argv[i]) == 0) {
            cout << "--help - shows that list" << endl;
            cout << "-in - specify input file" << endl;
            cout << "-out - specify output file" << endl;
            cout << "-p - read partial solution in the end of input" << endl;
            cout << "-f - format solution dividing it into 5x5 squares" << endl;
            cout << "-v - verbose progress" << endl;
        }
        if (strcmp("-in", argv[i]) == 0) {
            if (i + 1 < argc) {
                freopen(argv[i + 1], "r", stdin);
                i++;
            } else
                cout << "No input file specified.";
        }
        if (strcmp("-out", argv[i]) == 0) {
            if (i + 1 < argc) {
                freopen(argv[i + 1], "w", stdout);
                i++;
            } else
                cout << "No output file specified.";
        }
        if (strcmp("-p", argv[i]) == 0) {
            doReadPartial = true;
        }
        if (strcmp("-f", argv[i]) == 0) {
            doFormat = true;
        }
        if (strcmp("-v", argv[i]) == 0) {
            doVerbose = true;
        }
    }
    readTask();
    if (doReadPartial)
        readData();
    runTillStabilize(getProgress, boundsIntersectionMethod);
    bruteforce();
    while (!isSolved()) {
        runTillStabilize(getProgress, boundsIntersectionMethod);
        bruteforceSelect();
    }
    printProgress();
    printSolution(doFormat);
    return 0;
}
