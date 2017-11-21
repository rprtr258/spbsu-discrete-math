#include <stdio.h>

void readFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    char symbol = '\0';
    while (!feof(file)) {
        fscanf(file, "%c", &symbol);
        if (feof(file))
            continue;
        printf("%c", symbol);
    }
    fclose(file);
}

int main() {    
    readFile("encoded.txt");
    return 0;
}