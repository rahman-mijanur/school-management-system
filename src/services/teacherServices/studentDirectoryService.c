#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "models/student.h"
#include "models/teacher.h"
#include "services/teacherServices/studentDirectoryService.h"
#include "storage/databaseHandler.h"

static char* caseInsensitiveSubstring(const char *haystack, const char *needle) {
    if(!haystack || !needle) return NULL;
    if(!*needle) return (char *)haystack;

    for(; *haystack; ++haystack) {
        if(tolower((unsigned char)*haystack) == tolower((unsigned char)*needle)) {
            const char *h = haystack + 1;
            const char *n = needle + 1;
            while (*h && *n && tolower((unsigned char)*h) == tolower((unsigned char)*n)) {
                h++;
                n++;
            }
            if(!*n) return (char *)haystack;
        }
    }
    return NULL;
}

static bool isStudentAssignedToTeacher(int teacherId, const Student *std) {
    if(std == NULL || !std->isActive) return false;

    for(int i = 0; i < teacherCount; i++) {
        if(teacherDB[i].id == teacherId && teacherDB[i].isActive) {
            for(int j = 0; j < teacherDB[i].assignedSubjectCount; j++) {
                if(teacherDB[i].assignedSubjects[j].classNumber == std->currentClass) {
                    return true;
                }
            }
            break;
        }
    }
    return false;
}

int getTeacherStudentDirectory(int teacherId, Student *roster[], int maxStudents) {
    if(roster == NULL || maxStudents <= 0) return 0;

    int count = 0;
    for(int i = 0; i < studentCount; i++) {
        if(isStudentAssignedToTeacher(teacherId, &studentDB[i])) {
            if(count < maxStudents) {
                roster[count++] = &studentDB[i];
            }
        }
    }
    return count;
}

Student* findStudentInDirectoryById(int teacherId, int studentId) {
    for(int i = 0; i < studentCount; i++) {
        if(studentDB[i].id == studentId && isStudentAssignedToTeacher(teacherId, &studentDB[i])) {
            return &studentDB[i];
        }
    }
    return NULL;
}

int findStudentsInDirectoryByName(int teacherId, const char *query, Student *matches[], int maxMatches) {
    if(query == NULL || matches == NULL || maxMatches <= 0) return 0;

    int count = 0;
    for(int i = 0; i < studentCount; i++) {
        if(isStudentAssignedToTeacher(teacherId, &studentDB[i])) {
            if(caseInsensitiveSubstring(studentDB[i].name, query) != NULL) {
                matches[count++] = &studentDB[i];
                if(count >= maxMatches) break;
            }
        }
    }
    return count;
}
