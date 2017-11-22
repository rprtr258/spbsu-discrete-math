#pragma once
#include <utility>

typedef std::pair<unsigned char, int> CharOccur;

struct FrequencyTable {
    CharOccur *data = nullptr;
    int size = 0;
    
    FrequencyTable(const char *filename);
    FrequencyTable(const unsigned char *str);
    
    ~FrequencyTable();
    
    CharOccur& operator[](int const &index);
    const CharOccur& operator[](int const &index) const;
};
