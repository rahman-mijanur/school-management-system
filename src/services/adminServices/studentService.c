#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "utils/config.h"
#include "models/student.h"
#include "models/user.h"
#include "storage/databaseHandler.h"
#include "services/adminServices/studentService.h"

static char* caseInsensitiveSubstring(const char *haystack, const char *needle) {
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

static int getStudentDatabaseIndex(int id) {
    for(int i = 0; i < studentCount; i++) {
        if(studentDB[i].id == id) {
            return i;
        }
    }
    return -1;
}

bool registerStudent(Student student, UserAccount *createdUser) {
    if(studentCount >= MAX_STUDENTS || userCount >= MAX_USERS) return false;

    // Set auto-generated student fields
    student.id = nextStudentId;
    student.isActive = true;

    // Generate student User Account
    UserAccount newUser;
    memset(&newUser, 0, sizeof(UserAccount));

    snprintf(newUser.username, sizeof(newUser.username), "student_%d", student.id);
    snprintf(newUser.password, sizeof(newUser.password), "student123");
    newUser.role = ROLE_STUDENT;
    newUser.linkedId = student.id;
    newUser.isActive = true;

    // Save to memory
    studentDB[studentCount] = student;
    userDB[userCount] = newUser;

    studentCount++;
    userCount++;
    nextStudentId++;

    if(saveDatabase()) {
        if(createdUser != NULL) {
            *createdUser = newUser;
        }
        return true;
    } else {
        studentCount--;
        userCount--;
        nextStudentId--;
        return false;
    }
}

Student* findStudentById(int id) {
    int index = getStudentDatabaseIndex(id);
    if(index >= 0 && index < studentCount) {
        return &studentDB[index];
    }
    return NULL;
}

int findStudentsByName(const char *query, Student *matches[], int maxMatches) {
    if(query == NULL || matches == NULL || maxMatches <= 0) return 0;

    int matchCount = 0;
    for(int i = 0; i < studentCount; i++) {
        if(caseInsensitiveSubstring(studentDB[i].name, query) != NULL) {
            matches[matchCount] = &studentDB[i];
            matchCount++;
            if(matchCount >= maxMatches) break;
        }
    }
    return matchCount;
}

bool updateStudent(int studentId, Student updatedData) {
    int index = getStudentDatabaseIndex(studentId);
    if(index < 0) return false;

    Student backupData = studentDB[index];
    studentDB[index] = updatedData;

    if(saveDatabase()) {
        return true;
    } else {
        studentDB[index] = backupData;
        return false;
    }
}

bool deleteStudent(int studentId, bool *userAccountDeleted) {
    int targetIndex = getStudentDatabaseIndex(studentId);
    if(targetIndex < 0) return false;

    if(userAccountDeleted != NULL) {
        *userAccountDeleted = false;
    }

    // Remove user account
    for(int i = 0; i < userCount; i++) {
        if(userDB[i].role == ROLE_STUDENT && userDB[i].linkedId == studentId) {
            for(int j = i; j < userCount - 1; j++) {
                userDB[j] = userDB[j + 1];
            }
            userCount--;
            if(userAccountDeleted != NULL) {
                *userAccountDeleted = true;
            }
            break;
        }
    }

    // Remove student record
    for(int i = targetIndex; i < studentCount - 1; i++) {
        studentDB[i] = studentDB[i + 1];
    }
    studentCount--;

    return saveDatabase();
}
