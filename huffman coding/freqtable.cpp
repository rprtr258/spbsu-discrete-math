#include <string.h>
#include <algorithm>
#include "freqtable.h"

int unsigned const alphabet = 256;

CharOccur* createSimpleTable(std::vector<char unsigned> str) {
    int unsigned charCount[alphabet];
    memset(charCount, 0, alphabet * sizeof(int unsigned));
    
    for (char unsigned const &byte : str)
        charCount[byte]++;
    
    CharOccur *table = new CharOccur[alphabet];
    for (int unsigned i = 0; i < alphabet; i++)
        table[i] = CharOccur((char)i, charCount[i]);
    std::sort(table, table + alphabet, [](const CharOccur &occur1, const CharOccur &occur2) {
        return occur1.second < occur2.second;
    });
    
    return table;
}

FrequencyTable::FrequencyTable(std::vector<char unsigned> str) {
    CharOccur *simpleTable = createSimpleTable(str);
    
    int unsigned ptr = 0;
    while (simpleTable[ptr].second == 0)
        ptr++;
    
    size = alphabet - ptr;
    if (size == 1) {
        // TODO
    } else if (size > 1) {
        data = new CharOccur[size];
        for (int unsigned i = 0; i < size; i++)
            data[i] = simpleTable[i + ptr];
    }
    
    delete[] simpleTable;
}

FrequencyTable::~FrequencyTable() {
    if (data != nullptr)
        delete[] data;
}

CharOccur FrequencyTable::operator[](int unsigned const index) {
    if (index >= size)
        return {'\0', 0};
    return data[index];
}

int unsigned FrequencyTable::getSize() {
    return size;
}

void FrequencyTable::erase() {
    if (data != nullptr)
        delete[] data;
    data = nullptr;
    size = 0;
}
