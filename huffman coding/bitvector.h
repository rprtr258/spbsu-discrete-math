#pragma once

struct BitVector {
    unsigned char *data = nullptr;
    unsigned int size = 0;
    unsigned int capacity = 0;
    
    BitVector(unsigned int const _size = 0);
    BitVector(const BitVector &other);
    
    ~BitVector();
    
    bool get(unsigned int const &index) const;
    void set(unsigned int const &index, bool const &value);
    
    void pushBack(bool const &value);
    void pushBack(BitVector const &other);
    void popBack();
    
    void print();
    void printBytes();
    
    const BitVector& operator=(const BitVector &other);
};