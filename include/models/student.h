#ifndef STUDENT_H
#define STUDENT_H

#include <stdbool.h>
#include "utils/config.h"

typedef struct {
    int id;
    int rollNumber;
    char name[MAX_STR_LEN];
    char fatherName[MAX_STR_LEN];
    char motherName[MAX_STR_LEN];
    
    char email[MAX_STR_LEN];
    char phone[MAX_PHONE_LEN];
    char address[MAX_BUFFER];
    
    char dateOfBirth[MAX_DATE_LEN];
    char gender[10];
    char bloodGroup[5];
    
    int currentClass;
    char section[10];
    int academicYear;
    char admissionDate[MAX_DATE_LEN];

    bool isActive;
} Student;

#endif
