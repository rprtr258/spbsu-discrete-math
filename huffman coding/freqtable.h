#pragma once
#include <utility>

typedef std::pair<char, int unsigned> CharOccur;

class FrequencyTable {
    private:
        CharOccur *data = nullptr;
        int unsigned size = 0;
    public:
        FrequencyTable(const char *str);
        ~FrequencyTable();
        
        CharOccur operator[](int unsigned const index);
        
        int unsigned getSize();
        void erase();
};
