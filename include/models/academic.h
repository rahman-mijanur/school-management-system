#ifndef ACADEMIC_H
#define ACADEMIC_H

#include <stdbool.h>
#include "utils/config.h"

typedef struct {
    char subjectName[MAX_STR_LEN];
    float marksObtained;
    float maxMarks;
    float gradePoint;
    char letterGrade[5];
} Subject;

typedef struct {
    int id;
    int studentId;
    int classNumber;
    char examTerm[30];
    int year;
    Subject subjects[MAX_SUBJECTS];
    int subjectCount;
    float totalGPA;
    bool isActive;
} Result;

typedef struct {
    int examId;
    int classNumber;
    char subject[MAX_STR_LEN];
    char examDate[MAX_DATE_LEN];
    float totalMarks;
} Exam;

typedef struct {
    int id;
    int classNumber;
    char section[10];
    char dayOfWeek[15];
    int periodNumber;
    char timeSlot[30];
    char subjectName[MAX_STR_LEN];
    int teacherId;
} Routine;

typedef struct {
    int attendanceId;
    int studentId;
    int classNumber;
    char date[MAX_DATE_LEN];
    bool isPresent;
} Attendance;

typedef struct {
    int studentId;
    int rollNumber;
    char name[MAX_STR_LEN];
    int classNumber;
    int rank;
    float totalMarks;
    float percentage;
    float totalGPA;
    char letterGrade[5];
} MeritList;

typedef struct {
    int classNumber;
    int totalStudents;
    int academicYear;
} ClassSummary;

#endif
