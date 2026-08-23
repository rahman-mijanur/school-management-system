#ifndef TEACHER_H
#define TEACHER_H

#include <stdbool.h>
#include "utils/config.h"

typedef struct {
    int classNumber;
    char section[10];
    char subjectName[MAX_STR_LEN];
} SubjectAssignment;

typedef struct {
    int id;
    char name[MAX_STR_LEN];
    char email[MAX_STR_LEN];
    char phone[MAX_PHONE_LEN];
    char address[MAX_BUFFER];
    char qualification[MAX_STR_LEN];

    SubjectAssignment assignedSubjects[MAX_SUBJECTS];
    int assignedSubjectCount;

    float salary;
    int experience;
    char joiningDate[MAX_DATE_LEN];
    bool isActive;
} Teacher;

#endif
