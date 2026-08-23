#include <stdio.h>
#include <string.h>

#include "utils/colors.h"
#include "utils/config.h"
#include "utils/helpers.h"
#include "ui/common/components.h"
#include "ui/teacherUI/examManagementUI.h"
#include "services/teacherServices/examService.h"

static void showExamSchedulesUI(void) {
    clearScreen();
    printf("\n  " BG_BLUE "                                                                        " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "                      UPCOMING EXAM SCHEDULES                           " RESET "\n");
    printf("  " BG_BLUE "                                                                        " RESET "\n\n");

    Exam examList[MAX_EXAMS];
    int count = getAllExams(examList, MAX_EXAMS);

    if(count == 0) {
        printf(YELLOW "  No exams currently scheduled.\n" RESET);
    } else {
        printf(BOLD_WHITE "  %-10s %-10s %-20s %-15s %-10s\n" RESET, "Exam ID", "Class", "Subject", "Date", "Total Marks");
        printf("  ───────────────────────────────────────────────────────────────────────\n");

        for(int i = 0; i < count; i++) {
            printf("  %-10d %-10d %-20s %-15s %-10.2f\n", examList[i].examId, examList[i].classNumber, examList[i].subject, examList[i].examDate, examList[i].totalMarks);
        }
        printf("  ───────────────────────────────────────────────────────────────────────\n");
    }

    pauseAndClear();
}

static void createExamUI(void) {
    clearScreen();
    printf("\n");
    printf("  " BG_BLUE "                                                " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "                CREATE NEW EXAM                 " RESET "\n");
    printf("  " BG_BLUE "                                                " RESET "\n\n");

    int targetClass = getSafeInt("Enter Class (1-12)", 1, 12);
    char subject[MAX_STR_LEN];
    getSafeString(subject, sizeof(subject), "Enter Subject Name");

    char examDate[MAX_DATE_LEN];
    getSafeString(examDate, sizeof(examDate), "Enter Exam Date (YYYY-MM-DD)");

    float totalMarks = (float)getSafeInt("Enter Total Marks (10-100)", 10, 100);
    int createdId = createExam(targetClass, subject, examDate, totalMarks);

    if(createdId > 0) {
        printf("\n" GREEN "  ✔ Exam created successfully! (Exam ID: EX-%d)" RESET "\n", createdId);
    } else {
        showError("Failed to create exam record!");
    }

    pauseAndClear();
}

static void enterMarksUI(void) {
    clearScreen();
    printf("\n  " BG_BLUE "                                        " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "            ENTER EXAM MARKS            " RESET "\n");
    printf("  " BG_BLUE "                                        " RESET "\n\n");

    int examId = getSafeInt("Enter Exam ID", 1, 9999);
    int studentId = getSafeInt("Enter Student ID", 1, 999999);
    float obtainedMarks = (float)getSafeInt("Enter Obtained Marks", 0, 100);

    if(assignStudentMarks(examId, studentId, obtainedMarks)) {
        printf("\n" GREEN "  ✔ Marks updated for Student ID: %d" RESET "\n", studentId);
    } else {
        showError("Failed to record exam marks!");
    }

    pauseAndClear();
}

void manageExamsUI(void) {
    int running = 1;

    while(running) {
        clearScreen();

        printf("\n");
        printf("  " BG_BLUE "                                        " RESET "\n");
        printf("  " BG_BLUE BOLD_WHITE "        TEST & EXAM MANAGEMENT          " RESET "\n");
        printf("  " BG_BLUE "                                        " RESET "\n\n");

        printf(CYAN "  1." RESET " View All Exam Schedules\n");
        printf(CYAN "  2." RESET " Create New Exam Record\n");
        printf(CYAN "  3." RESET " Enter / Edit Exam Marks\n");
        printf(RED "  0." RESET " Back to Teacher Portal\n");
        printf("  ────────────────────────────────────────\n");

        int choice = getChoice();

        switch(choice) {
            case 0:
                running = 0;
                break;
            case 1:
                showExamSchedulesUI();
                break;
            case 2:
                createExamUI();
                break;
            case 3:
                enterMarksUI();
                break;
            default:
                showError("Invalid choice! Select 0-3.");
                pauseAndClear();
                break;
        }
    }
}
