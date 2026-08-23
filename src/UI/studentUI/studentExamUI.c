#include <stdio.h>
#include <string.h>

#include "models/academic.h"
#include "models/student.h"
#include "utils/colors.h"
#include "utils/helpers.h"
#include "ui/common/components.h"
#include "ui/studentUI/studentExamUI.h"
#include "storage/databaseHandler.h"

static Student* getStudentDetails(int studentId) {
    for(int i = 0; i < studentCount; i++) {
        if(studentDB[i].id == studentId && studentDB[i].isActive) {
            return &studentDB[i];
        }
    }
    return NULL;
}

static int getExamsByClass(int classNumber, Exam matches[], int maxMatches) {
    int count = 0;
    for(int i = 0; i < examCount; i++) {
        if(examDB[i].classNumber == classNumber) {
            if(count < maxMatches) {
                matches[count++] = examDB[i];
            }
        }
    }
    return count;
}

void showStudentExamUI(int studentId) {
    clearScreen();

    printf("\n");
    printf("  " BG_BLUE "                                                                " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "                       UPCOMING EXAM SCHEDULE                   " RESET "\n");
    printf("  " BG_BLUE "                                                                " RESET "\n\n");

    Student *std = getStudentDetails(studentId);

    if(std == NULL) {
        showError("Student profile details not found!");
        pauseAndClear();
        return;
    }

    Exam myExams[MAX_EXAMS];
    int count = getExamsByClass(std->currentClass, myExams, MAX_EXAMS);

    if(count == 0) {
        showError("No exam routine found for your class!");
        pauseAndClear();
        return;
    }

    printf(GREEN "  Student: %s " RESET " | " BOLD_WHITE "Class: %d (%s)" RESET "\n\n", std->name, std->currentClass, std->section);
    printf(BOLD_WHITE "  %-6s %-15s %-25s %-12s\n" RESET, "SL", "Date", "Subject", "Total Marks");
    printf("  ───────────────────────────────────────────────────────────────\n");
    for(int i = 0; i < count; i++) {
        printf("  %-6d %-15.15s %-25.25s %-12.2f\n", i + 1, myExams[i].examDate, myExams[i].subject, myExams[i].totalMarks);
    }
    printf("  ───────────────────────────────────────────────────────────────\n");
    printf(CYAN "  Total Scheduled Exams: %d\n" RESET, count);

    pauseAndClear();
}
