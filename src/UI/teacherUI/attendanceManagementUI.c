#include <stdio.h>
#include <string.h>

#include "utils/colors.h"
#include "utils/config.h"
#include "utils/helpers.h"
#include "models/student.h"
#include "models/academic.h"
#include "ui/common/components.h"
#include "ui/teacherUI/attendanceManagementUI.h"
#include "services/adminServices/studentService.h"
#include "services/teacherServices/attendanceService.h"
#include "storage/databaseHandler.h"

static void takeClassAttendanceUI(void) {
    clearScreen();
    printf("\n");
    printf("  " BG_BLUE "                                        " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "         TAKE CLASS ATTENDANCE          " RESET "\n");
    printf("  " BG_BLUE "                                        " RESET "\n\n");

    int targetClass = getSafeInt("Enter Class (1-12)", 1, 12);
    
    char dateStr[MAX_DATE_LEN];
    getSafeString(dateStr, sizeof(dateStr), "Enter Date (YYYY-MM-DD)");

    clearScreen();
    printf("\n");
    printf("  " BG_BLUE "                                                                             " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "                  ATTENDANCE SHEET - CLASS %-2d (%s)                   " RESET "\n", targetClass, dateStr);
    printf("  " BG_BLUE "                                                                             " RESET "\n\n");

    int foundStudents = 0;
    int presentCount = 0;

    printf(BOLD_WHITE "  %-8s %-10s %-25s %-15s\n" RESET, "ID", "Roll", "Name", "Status");
    printf("  ───────────────────────────────────────────────────────────────────────────────\n");

    for(int i = 0; i < studentCount; i++) {
        if(studentDB[i].currentClass == targetClass && studentDB[i].isActive) {
            foundStudents++;
            printf("  %-8d %-10d %-25.25s ", studentDB[i].id, studentDB[i].rollNumber, studentDB[i].name);

            int status = getSafeInt("[1: Present | 0: Absent]", 0, 1);
            bool isPresent = (status == 1);

            if(isPresent) presentCount++;

            markStudentAttendance(studentDB[i].id, targetClass, dateStr, isPresent);
        }
    }

    printf("  ───────────────────────────────────────────────────────────────────────────────\n");

    if(foundStudents == 0) {
        printf(YELLOW "  No active students enrolled in Class %d.\n" RESET, targetClass);
    } else {
        printf(GREEN "\n  ✔ Attendance recorded successfully!" RESET "\n");
        printf("  Total Present: %d / %d\n\n", presentCount, foundStudents);
    }

    pauseAndClear();
}

static void viewClassAttendanceReportUI(void) {
    clearScreen();
    printf("\n");
    printf("  " BG_BLUE "                                                                        " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "                        CLASS ATTENDANCE REPORT                         " RESET "\n");
    printf("  " BG_BLUE "                                                                        " RESET "\n\n");

    int targetClass = getSafeInt("Enter Class (1-12)", 1, 12);

    clearScreen();
    printf("\n  " BG_BLUE "                                                                              " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "                         ATTENDANCE SUMMARY - CLASS %-2d                        " RESET "\n", targetClass);
    printf("  " BG_BLUE "                                                                              " RESET "\n\n");

    printf(BOLD_WHITE "  %-10s %-6s %-20s %-12s %-14s %s\n" RESET, "ID", "Roll", "Name", "Attended", "Total Class", "Percentage");
    printf("  ──────────────────────────────────────────────────────────────────────────────\n");

    int count = 0;
    for(int i = 0; i < studentCount; i++) {
        if(studentDB[i].currentClass == targetClass && studentDB[i].isActive) {
            count++;

            int totalClasses = 0;
            int totalPresent = 0;
            for(int j = 0; j < attendanceCount; j++) {
                if(attendanceDB[j].studentId == studentDB[i].id) {
                    totalClasses++;
                    if(attendanceDB[j].isPresent) totalPresent++;
                }
            }
            float percentage = (totalClasses > 0) ? ((float)totalPresent / totalClasses) * 100.0f : 0.0f;

            printf("  %-10d %-6d %-20s %-12d %-14d %.1f%%\n",
                   studentDB[i].id,
                   studentDB[i].rollNumber,
                   studentDB[i].name,
                   totalPresent,
                   totalClasses,
                   percentage);
        }
    }
    printf("  ──────────────────────────────────────────────────────────────────────────────\n");

    if(count == 0) {
        printf(YELLOW "  No records found for Class %d.\n" RESET, targetClass);
    }

    pauseAndClear();
}

void manageAttendanceUI(void) {
    int running = 1;

    while(running) {
        clearScreen();

        printf("\n");
        printf("  " BG_BLUE "                                        " RESET "\n");
        printf("  " BG_BLUE BOLD_WHITE "         ATTENDANCE MANAGEMENT          " RESET "\n");
        printf("  " BG_BLUE "                                        " RESET "\n\n");

        printf(CYAN "  1." RESET " Take Daily Class Attendance\n");
        printf(CYAN "  2." RESET " View Class Attendance Report\n");
        printf(RED "  0." RESET " Back to Teacher Portal\n");
        printf("  ────────────────────────────────────────\n");

        int choice = getChoice();

        switch(choice) {
            case 0:
                running = 0;
                break;
            case 1:
                takeClassAttendanceUI();
                break;
            case 2:
                viewClassAttendanceReportUI();
                break;
            default:
                showError("Invalid choice! Select 0-2.");
                pauseAndClear();
                break;
        }
    }
}

void viewMyAttendanceUI(int studentId) {
    clearScreen();

    printf("\n");
    printf("  " BG_BLUE "                                                                                " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "                          MY ATTENDANCE TRACKING                                " RESET "\n");
    printf("  " BG_BLUE "                                                                                " RESET "\n\n");

    Student *s = findStudentById(studentId);

    if(s == NULL) {
        showError("Student record not found in database!");
        pauseAndClear();
        return;
    }

    int totalClasses = 0;
    int totalPresent = 0;
    for(int j = 0; j < attendanceCount; j++) {
        if(attendanceDB[j].studentId == studentId) {
            totalClasses++;
            if(attendanceDB[j].isPresent) totalPresent++;
        }
    }

    float percentage = (totalClasses > 0) ? ((float)totalPresent / totalClasses) * 100.0f : 0.0f;

    printf("  " BOLD_WHITE "Student Name :" RESET " %-25s | " BOLD_WHITE "Class :" RESET " %d (%s)\n", s->name, s->currentClass, s->section);
    printf("  " BOLD_WHITE "Total Conducted :" RESET " %-21d | " BOLD_WHITE "Present :" RESET " %d Day(s)\n", totalClasses, totalPresent);
    
    if(percentage >= 75.0f) {
        printf("  " BOLD_WHITE "Percentage   :" RESET " " GREEN "%.2f%% (Eligible)" RESET "\n", percentage);
    } else {
        printf("  " BOLD_WHITE "Percentage   :" RESET " " RED "%.2f%% (Below Warning Limit)" RESET "\n", percentage);
    }
    
    printf("  ──────────────────────────────────────────────────────────────────────────────\n\n");

    Attendance history[500];
    int count = getStudentAttendanceHistory(studentId, history, 500);

    if(count == 0) {
        printf("  " YELLOW "No individual class attendance logs recorded for your ID yet." RESET "\n\n");
        pauseAndClear();
        return;
    }

    printf(BOLD_WHITE "  %-12s %-15s %-15s %-15s\n" RESET, "Log ID", "Class Num", "Date", "Status");
    printf("  ──────────────────────────────────────────────────────────────────────────────\n");

    for(int i = 0; i < count; i++) {
        const char *statusStr = history[i].isPresent ? GREEN "PRESENT" RESET : RED "ABSENT" RESET;
        printf("  %-12d Class %-9d %-15s %s\n", history[i].attendanceId, history[i].classNumber, history[i].date, statusStr);
    }

    printf("  ──────────────────────────────────────────────────────────────────────────────\n\n");

    pauseAndClear();
}
