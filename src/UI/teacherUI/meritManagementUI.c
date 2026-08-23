#include <stdio.h>
#include <string.h>

#include "utils/colors.h"
#include "utils/config.h"
#include "utils/helpers.h"
#include "models/academic.h"
#include "ui/common/components.h"
#include "ui/teacherUI/meritManagementUI.h"
#include "services/teacherServices/meritService.h"

static void viewClassMeritListUI(void) {
    clearScreen();
    printf("\n");
    printf("  " BG_BLUE "                                        " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "            CLASS MERIT LIST            " RESET "\n");
    printf("  " BG_BLUE "                                        " RESET "\n\n");

    int targetClass = getSafeInt("Enter Class (1-12)", 1, 12);
    MeritList meritList[MAX_STUDENTS];
    int count = generateClassMeritList(targetClass, meritList, MAX_STUDENTS);

    clearScreen();
    printf("\n");
    printf("  " BG_BLUE "                                                                    " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "                      CLASS %-2d MERIT STANDINGS                      " RESET "\n", targetClass);
    printf("  " BG_BLUE "                                                                    " RESET "\n\n");

    if(count == 0) {
        printf(YELLOW "  No result records found for Class %d.\n" RESET, targetClass);
    } else {
        printf(BOLD_WHITE "  %-6s %-10s %-6s %-20s %-6s %-6s %-6s\n" RESET, "Rank", "ID", "Roll", "Name", "Marks", "GPA", "Grade");
        printf("  ────────────────────────────────────────────────────────────────────\n");

        for(int i = 0; i < count; i++) {
            printf("  %-6d %-10d %-6d %-20s %-6.2f %-6.2f %-6s\n",
                   meritList[i].rank,
                   meritList[i].studentId,
                   meritList[i].rollNumber,
                   meritList[i].name,
                   meritList[i].totalMarks,
                   meritList[i].totalGPA,
                   meritList[i].letterGrade);
        }
        printf("  ────────────────────────────────────────────────────────────────────\n");
        printf(GREEN "  Total Top Students Processed: %d\n" RESET, count);
    }

    pauseAndClear();
}

static void viewOverallSchoolMeritListUI(void) {
    clearScreen();
    printf("\n");
    printf("  " BG_BLUE "                                                                          " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "                          INSTITUTION MERIT LIST                          " RESET "\n");
    printf("  " BG_BLUE "                                                                          " RESET "\n\n");

    MeritList meritList[MAX_STUDENTS];
    int count = generateSchoolMeritList(meritList, MAX_STUDENTS);

    if(count == 0) {
        printf(YELLOW "  No academic records found in database.\n" RESET);
    } else {
        printf(BOLD_WHITE "  %-6s %-10s %-6s %-20s %-8s %-8s %-8s\n" RESET, "Rank", "ID", "Class", "Name", "Roll", "Marks", "GPA");
        printf("  ──────────────────────────────────────────────────────────────────────────\n");

        for(int i = 0; i < count; i++) {
            printf("  %-6d %-10d %-6d %-20s %-8d %-8.2f %-8.2f\n",
                   meritList[i].rank,
                   meritList[i].studentId,
                   meritList[i].classNumber,
                   meritList[i].name,
                   meritList[i].rollNumber,
                   meritList[i].totalMarks,
                   meritList[i].totalGPA);
        }
        printf("  ──────────────────────────────────────────────────────────────────────────\n");
        printf(GREEN "  Total Institution Rank Processed: %d\n" RESET, count);
    }

    pauseAndClear();
}

void manageMeritListUI(void) {
    int running = 1;

    while(running) {
        clearScreen();

        printf("\n");
        printf("  " BG_BLUE "                                        " RESET "\n");
        printf("  " BG_BLUE BOLD_WHITE "         MERIT-LIST MANAGEMENT          " RESET "\n");
        printf("  " BG_BLUE "                                        " RESET "\n\n");

        printf(CYAN "  1." RESET " View Class-wise Merit List\n");
        printf(CYAN "  2." RESET " View Institution Merit List\n");
        printf(RED "  0." RESET " Back to Teacher Portal\n");
        printf("  ────────────────────────────────────────\n");

        int choice = getChoice();

        switch(choice) {
            case 0:
                running = 0;
                break;
            case 1:
                viewClassMeritListUI();
                break;
            case 2:
                viewOverallSchoolMeritListUI();
                break;
            default:
                showError("Invalid choice! Select 0-2.");
                pauseAndClear();
                break;
        }
    }
}
