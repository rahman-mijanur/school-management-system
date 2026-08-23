#include <stdio.h>
#include <string.h>

#include "utils/colors.h"
#include "utils/helpers.h"
#include "models/student.h"
#include "models/academic.h"
#include "ui/common/components.h"
#include "ui/studentUI/academicReportUI.h"
#include "storage/databaseHandler.h"

static Student* getStudentDetails(int studentId) {
    for(int i = 0; i < studentCount; i++) {
        if(studentDB[i].id == studentId && studentDB[i].isActive) {
            return &studentDB[i];
        }
    }
    return NULL;
}

static int getStudentResults(int studentId, Result *matches[], int maxMatches) {
    int count = 0;
    for(int i = 0; i < resultCount; i++) {
        if(resultDB[i].studentId == studentId && resultDB[i].isActive) {
            if(count < maxMatches) {
                matches[count++] = &resultDB[i];
            }
        }
    }
    return count;
}

static const char* calculateFinalGrade(float gpa) {
    if(gpa >= 5.0f) return "A+";
    if(gpa >= 4.0f) return "A";
    if(gpa >= 3.5f) return "A-";
    if(gpa >= 3.0f) return "B";
    if(gpa >= 2.0f) return "C";
    if(gpa >= 1.0f) return "D";
    return "F";
}

static void displaySingleReportCard(const Result *res, const Student *std) {
    float totalObtainedMarks = 0.0f;
    float totalMaxMarks = 0.0f;

    for(int i = 0; i < res->subjectCount; i++) {
        totalObtainedMarks += res->subjects[i].marksObtained;
        totalMaxMarks += res->subjects[i].maxMarks;
    }

    float overallPercent = (totalMaxMarks > 0) ? (totalObtainedMarks / totalMaxMarks) * 100.0f : 0.0f;
    const char *finalGrade = calculateFinalGrade(res->totalGPA);

    printf("\n");
    printf("  " BOLD_WHITE "┌──────────────────────────────────────────────────────────────────────────────┐" RESET "\n");
    printf("  " BOLD_WHITE "│ " CYAN "EXAM TERM : %-27s " RESET "| " BOLD_WHITE "Year : %-27d │" RESET "\n", res->examTerm, res->year);
    
    if(std != NULL) {
        char classSec[30];
        snprintf(classSec, sizeof(classSec), "%d (%s)", std->currentClass, std->section);
        printf("  " BOLD_WHITE "│ " RESET "Student Name : %-24s | Class : %-26s " BOLD_WHITE "│" RESET "\n", std->name, classSec);
        printf("  " BOLD_WHITE "│ " RESET "Roll Number  : %-24d | ID    : %-26d " BOLD_WHITE "│" RESET "\n", std->rollNumber, std->id);
    }
    printf("  " BOLD_WHITE "├──────────────────────────────────────────────────────────────────────────────┤" RESET "\n");
    printf("  " BOLD_WHITE "│ " BOLD_WHITE "%-4s %-19s %-12s %-12s %-12s %-12s " BOLD_WHITE "│" RESET "\n", 
           "SL", "Subject Name", "Marks", "Max Marks", "Grade Point", "Grade Letter");
    printf("  " BOLD_WHITE "├──────────────────────────────────────────────────────────────────────────────┤" RESET "\n");

    for(int i = 0; i < res->subjectCount; i++) {
        printf("  " BOLD_WHITE "│" RESET " %-4d %-19s %-12.2f %-12.2f %-12.2f %-12s " BOLD_WHITE "│" RESET "\n",
               i + 1,
               res->subjects[i].subjectName,
               res->subjects[i].marksObtained,
               res->subjects[i].maxMarks,
               res->subjects[i].gradePoint,
               res->subjects[i].letterGrade);
    }

    printf("  " BOLD_WHITE "├──────────────────────────────────────────────────────────────────────────────┤" RESET "\n");
    printf("  " BOLD_WHITE "│ " GREEN "%-20s : %-53.2f " BOLD_WHITE "│" RESET "\n", "Total Marks", totalObtainedMarks);
    printf("  " BOLD_WHITE "│ " GREEN "%-20s : %-.2f%%%-47s " BOLD_WHITE "│" RESET "\n", "Overall Percent", overallPercent, "");
    printf("  " BOLD_WHITE "│ " GREEN "%-20s : %-53.2f " BOLD_WHITE "│" RESET "\n", "GPA / Grade Point", res->totalGPA);
    printf("  " BOLD_WHITE "│ " GREEN "%-20s : %-53s " BOLD_WHITE "│" RESET "\n", "Final Grade", finalGrade);
    printf("  " BOLD_WHITE "└──────────────────────────────────────────────────────────────────────────────┘" RESET "\n");
}

void showAcademicReportUI(int studentId) {
    clearScreen();

    printf("\n");
    printf("  " BG_BLUE "                                                                                " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "                              ACADEMIC REPORT CARD                              " RESET "\n");
    printf("  " BG_BLUE "                                                                                " RESET "\n\n");

    Student *std = getStudentDetails(studentId);
    Result *myResults[MAX_RESULTS];
    int count = getStudentResults(studentId, myResults, MAX_RESULTS);

    if(count == 0) {
        showError("No academic results or report cards found for your account!");
        pauseAndClear();
        return;
    }

    for(int i = 0; i < count; i++) {
        displaySingleReportCard(myResults[i], std);
    }

    pauseAndClear();
}
