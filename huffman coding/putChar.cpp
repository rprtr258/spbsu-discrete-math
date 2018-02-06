#include <string.h>
#include "putChar.h"

void putChar(char *&str, char const symbol) {
    int oldLength = strlen(str);
    char *newStr = new char[oldLength + 2];
    strcpy(newStr, str);
    newStr[oldLength] = symbol;
    newStr[oldLength + 1] = '\0';
    delete[] str;
    str = newStr;
}

char* strConcate(const char *str1, const char *str2) {
    int unsigned length1 = strlen(str1);
    int unsigned length2 = strlen(str2);
    char *result = new char[length1 + length2 + 1];
    strcpy(result, str1);
    strcpy(result + length1, str2);
    return result;
}

