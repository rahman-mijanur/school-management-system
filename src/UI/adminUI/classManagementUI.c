#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/colors.h"
#include "utils/config.h"
#include "utils/helpers.h"
#include "models/student.h"
#include "models/teacher.h"
#include "models/academic.h"

#include "ui/common/components.h"
#include "ui/adminUI/classManagementUI.h"
#include "services/adminServices/classService.h"
#include "storage/databaseHandler.h"

#define MAX_TEMP_CLASSES 50

static void viewAcademicClasses(void);
static void viewTeachersDirectory(void);

void showClassManagementMenu(void) {
    int running = 1;

    while(running) {
        clearScreen();
        printf("\n  " BG_BLUE "                                               " RESET "\n");
        printf("  " BG_BLUE BOLD_WHITE "           ACADEMIC CLASS MANAGEMENT           " RESET "\n");
        printf("  " BG_BLUE "                                               " RESET "\n\n");

        printf(CYAN "  1." RESET " View Active Classes\n");
        printf(CYAN "  2." RESET " View Class Roster Sheet\n");
        printf(CYAN "  3." RESET " View Teacher Directory (Subject-wise)\n");
        printf(RED  "  0." RESET " Back to Admin Dashboard\n");
        printf("  ───────────────────────────────────────────────\n");

        int choice = getChoice();

        switch(choice) {
            case 1:
                viewAcademicClasses();
                break;
            case 2:
                viewClassRoster();
                break;
            case 3:
                viewTeachersDirectory();
                break;
            case 0:
                running = 0;
                break;
            default:
                showError("Invalid choice! Please select 0 to 3.");
                pauseAndClear();
                break;
        }
    }
}

static void viewAcademicClasses(void) {
    clearScreen();
    printf("\n  " BG_BLUE "                                              " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "           ACADEMIC CLASSES OVERVIEW          " RESET "\n");
    printf("  " BG_BLUE "                                              " RESET "\n\n");

    ClassSummary classList[MAX_TEMP_CLASSES];
    int totalClasses = getClassList(classList, MAX_TEMP_CLASSES);

    if(totalClasses == 0) {
        showError("No active classes or enrolled students found in database.");
        pauseAndClear();
        return;
    }

    printf(BOLD_WHITE "  %-8s %-10s %-18s %-10s\n" RESET, "SL", "Class", "Total Enrolled", "Year");
    printf("  ──────────────────────────────────────────────\n");

    for(int i = 0; i < totalClasses; i++) {
        char className[30];
        snprintf(className, sizeof(className), "Class %d", classList[i].classNumber);

        printf("  %-8d %-10s %-18d %-10d\n", 
               i + 1, 
               className, 
               classList[i].totalStudents,
               classList[i].academicYear);
    }

    printf("  ──────────────────────────────────────────────\n");
    printf(GREEN "  Total Active Academic Sections: %d\n\n" RESET, totalClasses);

    pauseAndClear();
}

void viewClassRoster(void) {
    clearScreen();
    printf("\n  " BG_BLUE "                                                      " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "                  CLASS ROSTER SHEET                  " RESET "\n");
    printf("  " BG_BLUE "                                                      " RESET "\n\n");

    int targetClass = getSafeInt("Enter Class Number (1-12)", 1, 12);

    int studentIndices[MAX_STUDENTS];
    int count = getStudentsInClass(targetClass, studentIndices, MAX_STUDENTS);

    if(count == 0) {
        printf("\n  " RED "No active students currently enrolled in Class %d." RESET "\n\n", targetClass);
        pauseAndClear();
        return;
    }

    clearScreen();
    printf("\n  " BG_BLUE "                                                      " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "                  CLASS ROSTER SHEET                  " RESET "\n");
    printf("  " BG_BLUE "                                                      " RESET "\n\n");
    printf("  ──────────────────────────────────────────────────────\n");
    printf(BOLD_WHITE "  %-8s %-12s %-20s %-10s\n" RESET, "Roll", "ID", "Student Name", "Section");
    printf("  ──────────────────────────────────────────────────────\n");
    for(int i = 0; i < count; i++) {
        Student s = studentDB[studentIndices[i]];
        printf("  %-8d %-12d %-20s %-10s\n", s.rollNumber, s.id, s.name, s.section);
    }
    printf("  ──────────────────────────────────────────────────────\n");
    pauseAndClear();
}

static void viewTeachersDirectory(void) {
    clearScreen();
    printf("\n  " BG_BLUE "                                                                   " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "                         TEACHER DIRECTORY                         " RESET "\n");
    printf("  " BG_BLUE "                                                                   " RESET "\n\n");

    char subject[MAX_STR_LEN];
    getSafeString(subject, sizeof(subject), "Enter Subject Name");

    if(strlen(subject) == 0) {
        showError("Subject cannot be empty.");
        pauseAndClear();
        return;
    }

    int teacherIndices[MAX_TEACHERS];
    int count = getTeachersForSubject(subject, teacherIndices, MAX_TEACHERS);

    if(count == 0) {
        printf("\n  " RED "No teacher found for %s" RESET "\n\n", subject);
    } else {
        clearScreen();
        printf("  " BG_BLUE "                                                                   " RESET "\n");
        printf("  " BG_BLUE BOLD_WHITE "                         TEACHER DIRECTORY                         " RESET "\n");
        printf("  " BG_BLUE "                                                                   " RESET "\n\n");
        printf("  ───────────────────────────────────────────────────────────────────\n");
        printf(BOLD_WHITE "  %-6s %-22s %-25s %-15s\n" RESET, "ID", "Teacher Name", "Qualification", "Phone");
        printf("  ───────────────────────────────────────────────────────────────────\n");
        for(int i = 0; i < count; i++) {
            Teacher t = teacherDB[teacherIndices[i]];
            printf("  %-6d %-22s %-25s %-15s\n", t.id, t.name, t.qualification, t.phone);
        }
        printf("  ───────────────────────────────────────────────────────────────────\n");
    }

    pauseAndClear();
}
