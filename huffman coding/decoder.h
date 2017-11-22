#pragma once
#include "bitvector.h"

unsigned char* readFile(const char *filename);
unsigned char* decode(BitVector &code);
void encode(const char *inputFile, const char *outputFile);
void decode(const char *inputFile, const char *outputFile);