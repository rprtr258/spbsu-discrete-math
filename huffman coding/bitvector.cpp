#include <string.h>
#include <stdio.h>
#include "bitvector.h"

BitVector::BitVector(unsigned int const _size) {
    if (_size > 0) {
        size = _size;
        capacity = _size / 8 + (_size % 8 != 0);
        data = new char[capacity];
        memset(data, 0, capacity);
    }
}

BitVector::~BitVector() {
    delete data;
}

bool BitVector::get(unsigned int const &index) const {
    unsigned int byteIndex = index / 8;
    unsigned int bitIndex = 7 - index % 8;
    return (data[byteIndex] & (1 << bitIndex));
}

void BitVector::set(unsigned int const &index, bool const &value) {
    unsigned int byteIndex = index / 8;
    unsigned int bitIndex = 7 - index % 8;
    data[byteIndex] &= ~(1 << bitIndex);
    if (value)
        data[byteIndex] |= (1 << bitIndex);
}

void BitVector::pushBack(bool const &value) {
    if (size == capacity * 8) {
        char *newData = new char[capacity + 1];
        memset(newData, 0, capacity + 1);
        if (data != nullptr) {
            memcpy(newData, data, capacity);
            delete[] data;
        }
        data = newData;
        capacity++;
    }
    set(size, value);
    size++;
}

void BitVector::print() {
    for (unsigned int i = 0; i < size; i++)
        printf("%c", get(i) ? '1' : '0');
}

void BitVector::printBytes() {
    for (unsigned int i = 0; i < capacity; i++)
        printf("%c", data[i]);
}

void BitVector::pushBack(BitVector const &other) {
    // this->print();
    // printf(" + ");
    // other.print();
    for (unsigned int i = 0; i < other.size; i++)
        pushBack(other.get(i));
    // printf(" = ");
    // this->print();
    // printf("\n");
}

void BitVector::popBack() {
    set(size - 1, 0);
    size--;
}

const BitVector& BitVector::operator=(const BitVector &other) {
    delete[] data;
    
    size = other.size;
    capacity = other.capacity;
    
    data = new char[other.capacity];
    memcpy(data, other.data, capacity);
    
    return (*this);
}