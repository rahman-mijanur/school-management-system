#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <direct.h>
#include <windows.h>

#include "storage/fileHandler.h"

/* Windows Specific Directory Creation Macro */
#define MKDIR(path) _mkdir(path)

static void ensureParentDirExists(const char *filePath) {
    if(filePath == NULL) return;
    
    char dir[512];
    snprintf(dir, sizeof(dir), "%s", filePath);
    
    for(char *p = dir + 1; *p != '\0'; p++) {
        if(*p == '/' || *p == '\\') {
            if(p > dir && *(p - 1) == ':') continue;
            
            char sep = *p;
            *p = '\0';
            MKDIR(dir);
            *p = sep;
        }
    }
}

int readFromFile(const char *filePath, void *destBuffer, size_t elementSize, int maxElements) {
    if(filePath == NULL || destBuffer == NULL || elementSize == 0 || maxElements <= 0) return -1;
    
    FILE *file = fopen(filePath, "rb");
    if(file == NULL) return 0;
    
    size_t recordsRead = fread(destBuffer, elementSize, maxElements, file);
    if(ferror(file)) {
        fclose(file);
        return -1;
    }
    
    fclose(file);
    return (int)recordsRead;
}

bool writeToFile(const char *filePath, const void *srcBuffer, size_t elementSize, int elementCount) {
    if(filePath == NULL || (srcBuffer == NULL && elementCount > 0) || elementSize == 0 || elementCount < 0) return false;
    
    ensureParentDirExists(filePath);
    
    FILE *file = fopen(filePath, "wb");
    if(file == NULL) return false;
    
    if(elementCount == 0) {
        fclose(file);
        return true;
    }
    
    size_t recordsWritten = fwrite(srcBuffer, elementSize, elementCount, file);
    fflush(file);
    fclose(file);
    
    return recordsWritten == (size_t)elementCount;
}

bool writeToFileAtomic(const char *filePath, const void *srcBuffer, size_t elementSize, int elementCount) {
    if(filePath == NULL || (srcBuffer == NULL && elementCount > 0) || elementSize == 0 || elementCount < 0) return false;
    
    ensureParentDirExists(filePath);

    if(elementCount == 0) {
        FILE *f = fopen(filePath, "wb");
        if(f != NULL) {
            fclose(f);
            return true;
        }
        return false;
    }

    char tempPath[512];
    snprintf(tempPath, sizeof(tempPath), "%s.tmp", filePath);
    
    FILE *tempFile = fopen(tempPath, "wb");
    if(tempFile == NULL) return false;
    
    size_t recordsWritten = fwrite(srcBuffer, elementSize, elementCount, tempFile);
    bool writeSuccess = (recordsWritten == (size_t)elementCount);
    
    if(fflush(tempFile) != 0) writeSuccess = false;
    
    fclose(tempFile);
    
    if(!writeSuccess) {
        remove(tempPath);
        return false;
    }

    /* Native Win32 API for reliable atomic file replacement on Windows */
    if(MoveFileExA(tempPath, filePath, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) == 0) {
        if(rename(tempPath, filePath) != 0) {
            remove(tempPath);
            return false;
        }
    }
    
    return true;
}
