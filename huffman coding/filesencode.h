#pragma once

char* readFile(const char *filename);

int encodeFile(const char *fileInput, const char *fileOutput, const char *fileInfo);
int decodeFile(const char *fileInput, const char *fileOutput);
