#include <stdio.h>
#include <string.h>

#include "models/academic.h"
#include "models/student.h"
#include "utils/colors.h"
#include "utils/helpers.h"
#include "ui/common/components.h"
#include "ui/studentUI/studentAttendanceUI.h"
#include "storage/databaseHandler.h"

static Student* getStudentDetails(int studentId) {
    for(int i = 0; i < studentCount; i++) {
        if(studentDB[i].id == studentId && studentDB[i].isActive) {
            return &studentDB[i];
        }
    }
    return NULL;
}

static int getStudentAttendanceRecords(int studentId, Attendance matches[], int maxMatches) {
    int count = 0;
    for(int i = 0; i < attendanceCount; i++) {
        if(attendanceDB[i].studentId == studentId) {
            if(count < maxMatches) {
                matches[count++] = attendanceDB[i];
            }
        }
    }
    return count;
}

void showStudentAttendanceUI(int studentId) {
    clearScreen();

    printf("\n");
    printf("  " BG_BLUE "                                                                " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "                      ATTENDANCE TRACKING                       " RESET "\n");
    printf("  " BG_BLUE "                                                                " RESET "\n\n");

    Student *std = getStudentDetails(studentId);

    if(std == NULL) {
        showError("Student profile details not found!");
        pauseAndClear();
        return;
    }

    Attendance myAttendance[MAX_ATTENDANCE];
    int count = getStudentAttendanceRecords(studentId, myAttendance, MAX_ATTENDANCE);

    if(count == 0) {
        showError("No attendance records found for you!");
        pauseAndClear();
        return;
    }

    int presentCount = 0;
    int absentCount = 0;

    for(int i = 0; i < count; i++) {
        if(myAttendance[i].isPresent) {
            presentCount++;
        } else {
            absentCount++;
        }
    }

    float percentage = (count > 0) ? ((float)presentCount / count) * 100.0f : 0.0f;

    printf(GREEN "  Student: %s " RESET " | " BOLD_WHITE "Class: %d (%s)" RESET "\n\n", std->name, std->currentClass, std->section);

    printf("  " BOLD_WHITE "┌──────────────────────────────────────────────────────────────┐" RESET "\n");
    printf("  " BOLD_WHITE "│ " CYAN "ATTENDANCE SUMMARY OVERVIEW                                  " BOLD_WHITE "│" RESET "\n");
    printf("  " BOLD_WHITE "├──────────────────────────────────────────────────────────────┤" RESET "\n");
    printf("  " BOLD_WHITE "│" RESET "  %-22s : %-34d " BOLD_WHITE "│" RESET "\n", "Total Working Days", count);
    printf("  " BOLD_WHITE "│" RESET "  %-22s : " GREEN "%-35d" BOLD_WHITE "│" RESET "\n", "Days Present", presentCount);
    printf("  " BOLD_WHITE "│" RESET "  %-22s : " RED "%-35d" BOLD_WHITE "│" RESET "\n", "Days Absent", absentCount);
    printf("  " BOLD_WHITE "│" RESET "  %-22s : " BOLD_WHITE "%.2f%%%-28s" BOLD_WHITE "│" RESET "\n", "Attendance Rate", percentage, "");
    printf("  " BOLD_WHITE "└──────────────────────────────────────────────────────────────┘" RESET "\n\n");

    printf(BOLD_WHITE "  %-6s %-18s %-15s %-15s\n" RESET, "SL", "Date", "Class", "Status");
    printf("  ───────────────────────────────────────────────────────────────\n");
    for(int i = 0; i < count; i++) {
        char classSec[20];
        snprintf(classSec, sizeof(classSec), "Class %d", myAttendance[i].classNumber);
        printf("  %-6d %-18.18s %-15.15s ", i + 1, myAttendance[i].date, classSec);
        if(myAttendance[i].isPresent) {
            printf(GREEN "%-15s" RESET "\n", "Present");
        } else {
            printf(RED "%-15s" RESET "\n", "Absent");
        }
    }
    printf("  ───────────────────────────────────────────────────────────────\n");

    pauseAndClear();
}
