#include <stdio.h>
#include <string.h>

#include "models/student.h"
#include "models/academic.h"
#include "storage/databaseHandler.h"
#include "services/teacherServices/meritService.h"

static void swapEntries(MeritList *a, MeritList *b) {
    MeritList temp = *a;
    *a = *b;
    *b = temp;
}

int generateClassMeritList(int classNumber, MeritList *meritList, int maxStudents) {
    if(meritList == NULL || maxStudents <= 0) return 0;

    int count = 0;

    for(int i = 0; i < studentCount; i++) {
        if(studentDB[i].currentClass == classNumber && studentDB[i].isActive) {
            if(count < maxStudents) {
                meritList[count].studentId = studentDB[i].id;
                meritList[count].rollNumber = studentDB[i].rollNumber;
                strncpy(meritList[count].name, studentDB[i].name, sizeof(meritList[count].name) - 1);
                meritList[count].name[sizeof(meritList[count].name) - 1] = '\0';
                meritList[count].classNumber = studentDB[i].currentClass;

                float totalMarks = 0.0f;
                float totalGPA = 0.0f;
                int resultCount = 0;

                for(int j = 0; j < resultCount; j++) {
                    if(resultDB[j].studentId == studentDB[i].id && resultDB[j].isActive) {
                        for(int k = 0; k < resultDB[j].subjectCount; k++) {
                            totalMarks += resultDB[j].subjects[k].marksObtained;
                        }
                        totalGPA += resultDB[j].totalGPA;
                        resultCount++;
                    }
                }

                meritList[count].totalMarks = totalMarks;
                meritList[count].totalGPA = (resultCount > 0) ? (totalGPA / resultCount) : 0.0f;
                meritList[count].percentage = (totalMarks > 0) ? (totalMarks / 500.0f) * 100.0f : 0.0f;

                count++;
            }
        }
    }

    if(count == 0) return 0;

    for(int i = 0; i < count - 1; i++) {
        for(int j = 0; j < count - i - 1; j++) {
            if(meritList[j].totalMarks < meritList[j + 1].totalMarks) {
                swapEntries(&meritList[j], &meritList[j + 1]);
            }
        }
    }

    for(int i = 0; i < count; i++) {
        meritList[i].rank = i + 1;
    }

    return count;
}

int generateSchoolMeritList(MeritList *meritList, int maxStudents) {
    if(meritList == NULL || maxStudents <= 0) return 0;

    int count = 0;

    for(int i = 0; i < studentCount; i++) {
        if(studentDB[i].isActive) {
            if(count < maxStudents) {
                meritList[count].studentId = studentDB[i].id;
                meritList[count].rollNumber = studentDB[i].rollNumber;
                strncpy(meritList[count].name, studentDB[i].name, sizeof(meritList[count].name) - 1);
                meritList[count].name[sizeof(meritList[count].name) - 1] = '\0';
                meritList[count].classNumber = studentDB[i].currentClass;

                float totalMarks = 0.0f;
                float totalGPA = 0.0f;
                int resultCount = 0;

                for(int j = 0; j < resultCount; j++) {
                    if(resultDB[j].studentId == studentDB[i].id && resultDB[j].isActive) {
                        for(int k = 0; k < resultDB[j].subjectCount; k++) {
                            totalMarks += resultDB[j].subjects[k].marksObtained;
                        }
                        totalGPA += resultDB[j].totalGPA;
                        resultCount++;
                    }
                }

                meritList[count].totalMarks = totalMarks;
                meritList[count].totalGPA = (resultCount > 0) ? (totalGPA / resultCount) : 0.0f;
                meritList[count].percentage = (totalMarks > 0) ? (totalMarks / 500.0f) * 100.0f : 0.0f;

                count++;
            }
        }
    }

    if(count == 0) return 0;

    for(int i = 0; i < count - 1; i++) {
        for(int j = 0; j < count - i - 1; j++) {
            if(meritList[j].totalMarks < meritList[j + 1].totalMarks) {
                swapEntries(&meritList[j], &meritList[j + 1]);
            }
        }
    }

    for(int i = 0; i < count; i++) {
        meritList[i].rank = i + 1;
    }

    return count;
}
