#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <stddef.h>
#include <stdbool.h>

int readFromFile(const char *filePath, void *destBuffer, size_t elementSize, int maxElements);
bool writeToFile(const char *filePath, const void *srcBuffer, size_t elementSize, int elementCount);
bool writeToFileAtomic(const char *filePath, const void *srcBuffer, size_t elementSize, int elementCount);

#endif
