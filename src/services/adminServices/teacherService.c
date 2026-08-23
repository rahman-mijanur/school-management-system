#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "utils/config.h"
#include "utils/helpers.h"
#include "models/teacher.h"
#include "models/user.h"
#include "storage/databaseHandler.h"
#include "services/adminServices/teacherService.h"

static char* customStristr(const char *haystack, const char *needle) {
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

static int searchTeacherIndexById(int id) {
    for(int i = 0; i < teacherCount; i++) {
        if(teacherDB[i].id == id) {
            return i;
        }
    }
    return -1;
}

bool registerTeacher(Teacher teacher, UserAccount *createdUser) {
    if(teacherCount >= MAX_TEACHERS || userCount >= MAX_USERS) return false;

    // Auto-generate fields
    teacher.id = nextTeacherId;
    teacher.isActive = true;

    // Generate User Account for Teacher
    UserAccount newUser;
    memset(&newUser, 0, sizeof(UserAccount));

    snprintf(newUser.username, sizeof(newUser.username), "teacher_%d", teacher.id);
    snprintf(newUser.password, sizeof(newUser.password), "teacher123");
    newUser.role = ROLE_TEACHER;
    newUser.linkedId = teacher.id;
    newUser.isActive = true;

    // Push to Memory
    teacherDB[teacherCount] = teacher;
    userDB[userCount] = newUser;

    teacherCount++;
    userCount++;
    nextTeacherId++;

    if(saveDatabase()) {
        if(createdUser != NULL) {
            *createdUser = newUser;
        }
        return true;
    } else {
        teacherCount--;
        userCount--;
        nextTeacherId--;
        return false;
    }
}

Teacher* searchTeacherById(int id) {
    int index = searchTeacherIndexById(id);

    if(index >= 0 && index < teacherCount) {
        return &teacherDB[index];
    }
    
    return NULL;
}

int searchTeachersByName(const char *query, Teacher *matches[], int maxMatches) {
    if(query == NULL || matches == NULL || maxMatches <= 0) return 0;

    int matchCount = 0;
    for(int i = 0; i < teacherCount; i++) {
        if(customStristr(teacherDB[i].name, query) != NULL) {
            matches[matchCount] = &teacherDB[i];
            matchCount++;
            if(matchCount >= maxMatches) break;
        }
    }
    return matchCount;
}

bool updateTeacher(int teacherId, Teacher updatedData) {
    int index = searchTeacherIndexById(teacherId);
    if(index < 0) return false;

    Teacher originalData = teacherDB[index];
    teacherDB[index] = updatedData;

    if(saveDatabase()) {
        return true;
    } else {
        teacherDB[index] = originalData;
        return false;
    }
}

bool deleteTeacher(int teacherId, bool *userDeletedOut) {
    int targetIndex = searchTeacherIndexById(teacherId);
    if(targetIndex < 0) return false;

    if(userDeletedOut != NULL) {
        *userDeletedOut = false;
    }

    // Remove associated user credential account
    for(int i = 0; i < userCount; i++) {
        if(userDB[i].role == ROLE_TEACHER && userDB[i].linkedId == teacherId) {
            for(int j = i; j < userCount - 1; j++) {
                userDB[j] = userDB[j + 1];
            }
            userCount--;
            if(userDeletedOut != NULL) {
                *userDeletedOut = true;
            }
            break;
        }
    }

    // Remove Teacher Profile from Array
    for(int i = targetIndex; i < teacherCount - 1; i++) {
        teacherDB[i] = teacherDB[i + 1];
    }
    teacherCount--;

    return saveDatabase();
}
