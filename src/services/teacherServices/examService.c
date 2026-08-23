#include <stdio.h>
#include <string.h>

#include "storage/databaseHandler.h"
#include "services/teacherServices/examService.h"

int getAllExams(Exam *examList, int maxExams) {
    if(examList == NULL || maxExams <= 0) return 0;

    int count = (examCount < maxExams) ? examCount : maxExams;
    for(int i = 0; i < count; i++) {
        examList[i] = examDB[i];
    }
    return count;
}

int createExam(int classNumber, const char *subject, const char *examDate, float totalMarks) {
    if(examCount >= MAX_EXAMS) return 0;

    int newId = 100 + examCount + 1;
    
    examDB[examCount].examId = newId;
    examDB[examCount].classNumber = classNumber;
    
    strncpy(examDB[examCount].subject, subject, sizeof(examDB[examCount].subject) - 1);
    examDB[examCount].subject[sizeof(examDB[examCount].subject) - 1] = '\0';

    strncpy(examDB[examCount].examDate, examDate, sizeof(examDB[examCount].examDate) - 1);
    examDB[examCount].examDate[sizeof(examDB[examCount].examDate) - 1] = '\0';

    examDB[examCount].totalMarks = totalMarks;

    examCount++;
    saveDatabase();
    return newId;
}

bool assignStudentMarks(int examId, int studentId, float marks) {
    if(examId <= 0 || studentId <= 0 || marks < 0) return false;

    bool studentFound = false;
    for(int i = 0; i < studentCount; i++) {
        if(studentDB[i].id == studentId && studentDB[i].isActive) {
            studentFound = true;
            break;
        }
    }
    if(!studentFound) return false;

    int examIndex = -1;
    for(int i = 0; i < examCount; i++) {
        if(examDB[i].examId == examId) {
            examIndex = i;
            break;
        }
    }
    if(examIndex == -1) return false;

    if(marks > examDB[examIndex].totalMarks) return false;

    int resultIndex = -1;
    for(int i = 0; i < resultCount; i++) {
        if(resultDB[i].studentId == studentId && resultDB[i].classNumber == examDB[examIndex].classNumber) {
            resultIndex = i;
            break;
        }
    }

    if(resultIndex != -1) {
        bool subjectFound = false;
        for(int j = 0; j < resultDB[resultIndex].subjectCount; j++) {
            if(strcmp(resultDB[resultIndex].subjects[j].subjectName, examDB[examIndex].subject) == 0) {
                resultDB[resultIndex].subjects[j].marksObtained = marks;
                subjectFound = true;
                break;
            }
        }
        if(!subjectFound && resultDB[resultIndex].subjectCount < MAX_SUBJECTS) {
            int subIdx = resultDB[resultIndex].subjectCount;
            strncpy(resultDB[resultIndex].subjects[subIdx].subjectName, examDB[examIndex].subject, MAX_STR_LEN - 1);
            resultDB[resultIndex].subjects[subIdx].marksObtained = marks;
            resultDB[resultIndex].subjects[subIdx].maxMarks = examDB[examIndex].totalMarks;
            resultDB[resultIndex].subjectCount++;
        }
    } else if(resultCount < MAX_RESULTS) {
        resultDB[resultCount].id = resultCount + 1;
        resultDB[resultCount].studentId = studentId;
        resultDB[resultCount].classNumber = examDB[examIndex].classNumber;
        resultDB[resultCount].subjectCount = 1;
        resultDB[resultCount].isActive = true;

        strncpy(resultDB[resultCount].subjects[0].subjectName, examDB[examIndex].subject, MAX_STR_LEN - 1);
        resultDB[resultCount].subjects[0].marksObtained = marks;
        resultDB[resultCount].subjects[0].maxMarks = examDB[examIndex].totalMarks;
        
        resultCount++;
    }

    saveDatabase();
    return true;
}
