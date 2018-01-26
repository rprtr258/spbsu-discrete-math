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
