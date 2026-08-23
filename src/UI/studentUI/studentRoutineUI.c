#include <stdio.h>
#include <string.h>

#include "utils/colors.h"
#include "utils/helpers.h"
#include "models/student.h"
#include "models/academic.h"
#include "ui/common/components.h"
#include "ui/studentUI/studentRoutineUI.h"
#include "services/adminServices/routineService.h"
#include "storage/databaseHandler.h"

static Student* getStudentDetails(int studentId) {
    for(int i = 0; i < studentCount; i++) {
        if(studentDB[i].id == studentId && studentDB[i].isActive) {
            return &studentDB[i];
        }
    }
    return NULL;
}

void showStudentRoutineUI(int studentId) {
    clearScreen();

    printf("\n");
    printf("  " BG_BLUE "                                                                              " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "                               MY CLASS ROUTINE                               " RESET "\n");
    printf("  " BG_BLUE "                                                                              " RESET "\n\n");

    Student *std = getStudentDetails(studentId);

    if(std == NULL) {
        showError("Student record not found in database!");
        pauseAndClear();
        return;
    }

    Routine myRoutines[MAX_ROUTINES];
    int count = getRoutineByClass(std->currentClass, std->section, myRoutines, MAX_ROUTINES);

    if(count == 0) {
        showError("No class routine entries found for you!");
        pauseAndClear();
        return;
    }

    printf(GREEN "  Student: %s " RESET " | " BOLD_WHITE "Class: %d (%s)" RESET "\n\n", std->name, std->currentClass, std->section);
    printf(BOLD_WHITE "  %-12s %-8s %-18s %-18s %-20s\n" RESET, "Day", "Period", "Time Slot", "Subject", "Teacher");
    printf("  ─────────────────────────────────────────────────────────────────────────────\n");

    for(int i = 0; i < count; i++) {
        const char *teacherName = getTeacherNameById(myRoutines[i].teacherId);
        printf("  %-12s %-8d %-18s %-18s %-20s\n",
               myRoutines[i].dayOfWeek,
               myRoutines[i].periodNumber,
               myRoutines[i].timeSlot,
               myRoutines[i].subjectName,
               teacherName);
    }
    printf("  ─────────────────────────────────────────────────────────────────────────────\n");
    printf(CYAN "  Total Scheduled Classes: %d\n" RESET, count);

    pauseAndClear();
}
