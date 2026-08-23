#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "utils/config.h"
#include "utils/helpers.h"
#include "models/student.h"
#include "models/teacher.h"
#include "models/academic.h"
#include "storage/databaseHandler.h"
#include "services/adminServices/classService.h"

static char* customStristr(const char *haystack, const char *needle) {
    if(!*needle) return (char *)haystack;
    
    for(; *haystack; ++haystack) {
        if(tolower((unsigned char)*haystack) == tolower((unsigned char)*needle)) {
            const char *h = haystack + 1;
            const char *n = needle + 1;
            while(*h && *n && tolower((unsigned char)*h) == tolower((unsigned char)*n)) {
                h++;
                n++;
            }
            if(!*n) return (char *)haystack;
        }
    }
    return NULL;
}

int getClassList(ClassSummary classList[], int maxClasses) {
    int uniqueClassCount = 0;

    for(int i = 0; i < studentCount; i++) {
        if(!studentDB[i].isActive) continue;

        int targetClass = studentDB[i].currentClass;
        if(targetClass <= 0) continue;

        int foundIdx = -1;
        for(int j = 0; j < uniqueClassCount; j++) {
            if(classList[j].classNumber == targetClass) {
                foundIdx = j;
                break;
            }
        }

        if(foundIdx != -1) {
            classList[foundIdx].totalStudents++;
        } else if(uniqueClassCount < maxClasses) {
            classList[uniqueClassCount].classNumber = targetClass;
            classList[uniqueClassCount].totalStudents = 1;
            classList[uniqueClassCount].academicYear = studentDB[i].academicYear;
            uniqueClassCount++;
        }
    }

    return uniqueClassCount;
}

int getStudentsInClass(int classNum, int studentIndices[], int maxBuffer) {
    int count = 0;
    for(int i = 0; i < studentCount; i++) {
        if(studentDB[i].isActive && studentDB[i].currentClass == classNum) {
            if(count < maxBuffer) {
                studentIndices[count++] = i;
            }
        }
    }
    return count;
}

int getTeachersForSubject(const char *subjectName, int teacherIndices[], int maxBuffer) {
    if(subjectName == NULL) return 0;
    
    int count = 0;
    for(int i = 0; i < teacherCount; i++) {
        if(!teacherDB[i].isActive) continue;

        // Qualification check or assigned subject match
        if(customStristr(teacherDB[i].qualification, subjectName) != NULL) {
            if(count < maxBuffer) {
                teacherIndices[count++] = i;
            }
        }
    }
    return count;
}
