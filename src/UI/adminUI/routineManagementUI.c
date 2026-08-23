#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>

#include "utils/colors.h"
#include "utils/config.h"
#include "utils/helpers.h"
#include "models/teacher.h"
#include "models/academic.h"
#include "ui/common/components.h"
#include "ui/adminUI/routineManagementUI.h"
#include "services/adminServices/routineService.h"
#include "storage/databaseHandler.h"

static int isValidDayInput(const char *day) {
    const char *days[] = {
        "saturday", "sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "sat", "sun", "mon", "tue", "wed", "thu", "fri"
    };
    for(int i = 0; i < 14; i++) {
        if(strcasecmp(day, days[i]) == 0) return 1;
    }
    return 0;
}

static int isTeacherIdValid(int teacherId) {
    for(int i = 0; i < teacherCount; i++) {
        if(teacherDB[i].id == teacherId && teacherDB[i].isActive) return 1;
    }
    return 0;
}

static void viewStudentClassRoutineUI(void) {
    clearScreen();
    printf("\n");
    printf("  " BG_BLUE "                                               " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "             STUDENT CLASS ROUTINE             " RESET "\n");
    printf("  " BG_BLUE "                                               " RESET "\n");
    printf("\n");

    int targetClass = getSafeInt("Enter Class (1-12)", 1, 12);
    char targetSection[10];
    getSafeString(targetSection, sizeof(targetSection), "Enter Section Name");

    Routine routine[MAX_ROUTINES];
    int count = getRoutineByClass(targetClass, targetSection, routine, MAX_ROUTINES);

    if(count == 0) {
        printf("  " RED "No routine slots configured for Class %d (%s)." RESET "\n", targetClass, targetSection);
        pauseAndClear();
        return;
    }

    clearScreen();
    printf("\n");
    printf("  " BG_BLUE "                                                                                       " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "                                 STUDENT CLASS ROUTINE                                 " RESET "\n");
    printf("  " BG_BLUE "                                                                                       " RESET "\n");
    printf("\n");


    printf("  " BOLD_WHITE "Class %d   [Section %s]" RESET "\n", targetClass, targetSection);
    printf("  ───────────────────────────────────────────────────────────────────────────────────────\n");
    printf(BOLD_WHITE "  %-6s %-10s %-10s %-20s %-15s %-25s\n" RESET, "ID", "Day", "Period", "Time Slot", "Subject", "Teacher Name");
    printf("  ───────────────────────────────────────────────────────────────────────────────────────\n");
    for(int i = 0; i < count; i++) {
        printf("  %-6d %-10s %-10d %-20s %-15s %-15s\n",
               routine[i].id, routine[i].dayOfWeek, routine[i].periodNumber,
               routine[i].timeSlot, routine[i].subjectName, getTeacherNameById(routine[i].teacherId));
    }
    printf("  ───────────────────────────────────────────────────────────────────────────────────────\n");
    pauseAndClear();
}

static void viewTeacherRoutineUI(void) {
    clearScreen();
    printf("\n");
    printf("  " BG_BLUE "                                              " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "          TEACHER INDIVIDUAL ROUTINE          " RESET "\n");
    printf("  " BG_BLUE "                                              " RESET "\n");
    printf("\n");

    int teacherId = getSafeInt("Enter Teacher ID", 1, 999999);

    if(!isTeacherIdValid(teacherId)) {
        showError("Teacher ID not found in database!");
        pauseAndClear();
        return;
    }

    Routine routine[MAX_ROUTINES];
    int count = getRoutineByTeacher(teacherId, routine, MAX_ROUTINES);

    if(count == 0) {
        printf("\n  " YELLOW "No routine assigned to Teacher: %s (ID: %d)" RESET "\n", getTeacherNameById(teacherId), teacherId);
        pauseAndClear();
        return;
    }

    clearScreen();
    printf("\n");
    printf("  " BG_BLUE "                                                                                      " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "                              TEACHER INDIVIDUAL ROUTINE                              " RESET "\n");
    printf("  " BG_BLUE "                                                                                      " RESET "\n");
    printf("\n");

    printf("\n  " BOLD_WHITE "ASSIGNED FOR: %s (ID: %d)" RESET "\n", getTeacherNameById(teacherId), teacherId);
    printf("  ──────────────────────────────────────────────────────────────────────────────────────\n");
    printf(BOLD_WHITE "  %-6s %-10s %-10s %-20s %-10s %-10s %-15s\n" RESET, "ID", "Day", "Period", "Time Slot", "Class", "Section", "Subject");
    printf("  ──────────────────────────────────────────────────────────────────────────────────────\n");
    for(int i = 0; i < count; i++) {
        printf("  %-6d %-10s %-10d %-20s %-10d %-10s %-15s\n",
               routine[i].id, routine[i].dayOfWeek, routine[i].periodNumber,
               routine[i].timeSlot, routine[i].classNumber, routine[i].section, routine[i].subjectName);
    }
    printf("  ──────────────────────────────────────────────────────────────────────────────────────\n");
    pauseAndClear();
}

static void viewCentralRoutineUI(void) {
    clearScreen();
    printf("\n");
    printf("  " BG_BLUE "                                                                                                          " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "                                          CENTRAL SCHOOL ROUTINE                                          " RESET "\n");
    printf("  " BG_BLUE "                                                                                                          " RESET "\n\n");

    Routine routine[MAX_ROUTINES];
    int count = getAllRoutines(routine, MAX_ROUTINES);

    if(count == 0) {
        printf("  " RED "No routine entries found in central database." RESET "\n");
        pauseAndClear();
        return;
    }

    printf(BOLD_WHITE "  %-6s %-10s %-6s %-10s %-10s %-20s %-15s %-20s\n" RESET, 
           "ID", "Day", "Class", "Section", "Period", "Time Slot", "Subject", "Teacher");
    printf("  ─────────────────────────────────────────────────────────────────────────────────────────────────────────\n");
    for(int i = 0; i < count; i++) {
        printf("  %-6d %-10s %-6d %-10s %-10d %-20s %-15s %-20s\n",
               routine[i].id, routine[i].dayOfWeek, routine[i].classNumber,
               routine[i].section, routine[i].periodNumber, routine[i].timeSlot, 
               routine[i].subjectName, getTeacherNameById(routine[i].teacherId));
    }
    printf("  ─────────────────────────────────────────────────────────────────────────────────────────────────────────\n");
    printf(GREEN "  Total Central Routine Slots Active: %d\n\n" RESET, count);
    pauseAndClear();
}

static void addRoutineSlotUI(void) {
    clearScreen();
    printf("\n  " BG_BLUE "                                                " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "             ADD NEW ROUTINE SLOT               " RESET "\n");
    printf("  " BG_BLUE "                                                " RESET "\n\n");

    Routine newSlot;
    memset(&newSlot, 0, sizeof(Routine));

    newSlot.classNumber = getSafeInt("Enter Class Number (1-12)", 1, 12);
    getSafeString(newSlot.section, sizeof(newSlot.section), "Enter Section");

    while(1) {
        getSafeString(newSlot.dayOfWeek, sizeof(newSlot.dayOfWeek), "Enter Day");
        if(isValidDayInput(newSlot.dayOfWeek)) break;
        showError("Invalid Day! Enter a valid day name (e.g., Sunday, Monday).");
    }

    newSlot.periodNumber = getSafeInt("Enter Period Number", 1, 10);
    getSafeString(newSlot.timeSlot, sizeof(newSlot.timeSlot), "Enter Time Slot");
    getSafeString(newSlot.subjectName, sizeof(newSlot.subjectName), "Enter Subject Name");

    while(1) {
        newSlot.teacherId = getSafeInt("Enter Assigned Teacher ID", 1, 999999);
        if(isTeacherIdValid(newSlot.teacherId)) {
            printf(GREEN "\n  Assigned Teacher: %s\n" RESET, getTeacherNameById(newSlot.teacherId));
            break;
        } else {
            showError("Teacher not found in database! Please enter a valid ID.");
        }
    }

    if(isClassBusy(newSlot)) {
        showError("This slot already booked!");
    } 
    else if (isTeacherBusy(newSlot)) {
        showError("This teacher is already assigned to another class at this Period!");
    } 
    else if(addRoutineEntry(newSlot)) {
        printf("\n" GREEN "  ✔ Routine slot added successfully!" RESET "\n");
    } 
    else {
        showError("Failed to save routine slot! Limit reached or storage write error.");
    }

    pauseAndClear();
}

static void removeRoutineSlotUI(void) {
    clearScreen();
    printf("\n");
    printf("  " BG_RED "                                              " RESET "\n");
    printf("  " BG_RED BOLD_WHITE "             REMOVE ROUTINE SLOT              " RESET "\n");
    printf("  " BG_RED "                                              " RESET "\n\n");

    int routineId = getSafeInt("Enter Routine Slot ID to remove", 1, 999999);

    if(deleteRoutineEntry(routineId)) {
        printf("\n" GREEN "  ✔ Routine slot removed successfully!" RESET "\n");
    } else {
        showError("Routine Slot ID not found or error updating database.");
    }
    pauseAndClear();
}

void manageRoutines(void) {
    int running = 1;
    
    while(running) {
        clearScreen();
        printf("\n");
        printf("  " BG_BLUE "                                        " RESET "\n");
        printf("  " BG_BLUE BOLD_WHITE "          ROUTINE MANAGEMENT            " RESET "\n");
        printf("  " BG_BLUE "                                        " RESET "\n\n");

        printf(CYAN "  1." RESET " View Student Class Routine\n");
        printf(CYAN "  2." RESET " View Teacher Routine\n");
        printf(CYAN "  3." RESET " View Central Routine\n");
        printf(CYAN "  4." RESET " Add New Routine Slot\n");
        printf(CYAN "  5." RESET " Remove Routine Slot\n");
        printf(RED  "  0." RESET " Back to Admin Dashboard\n");
        printf("  ────────────────────────────────────────\n\n");

        int choice = getChoice();

        switch(choice) {
            case 0:
                running = 0;
                break;
            case 1:
                viewStudentClassRoutineUI();
                break;
            case 2:
                viewTeacherRoutineUI();
                break;
            case 3:
                viewCentralRoutineUI();
                break;
            case 4:
                addRoutineSlotUI();
                break;
            case 5:
                removeRoutineSlotUI();
                break;
            default:
                showError("Invalid choice! Select 0 to 5.");
                pauseAndClear();
                break;
        }
    }
}

void viewTeacherRoutineByIdUI(int teacherId) {
    clearScreen();
    printf("\n");
    printf("  " BG_BLUE "                                                                                " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "                               MY CLASS ROUTINE                                 " RESET "\n");
    printf("  " BG_BLUE "                                                                                " RESET "\n\n");

    Routine routine[MAX_ROUTINES];
    int count = getRoutineByTeacher(teacherId, routine, MAX_ROUTINES);

    if(count == 0) {
        printf("\n  " YELLOW "No routine slots assigned to your Teacher ID (%d)." RESET "\n", teacherId);
        pauseAndClear();
        return;
    }

    printf("  " BOLD_WHITE "ASSIGNED CLASSES FOR: %s (ID: %d)" RESET "\n", getTeacherNameById(teacherId), teacherId);
    printf("  ────────────────────────────────────────────────────────────────────────────────\n");
    printf(BOLD_WHITE "  %-6s %-10s %-8s %-20s %-10s %-8s %-15s\n" RESET, "ID", "Day", "Period", "Time Slot", "Class", "Sec", "Subject");
    printf("  ────────────────────────────────────────────────────────────────────────────────\n");

    for(int i = 0; i < count; i++) {
        printf("  %-6d %-10s Period %-2d %-20s Class %-4d %-8s %-15.15s\n",
               routine[i].id, routine[i].dayOfWeek, routine[i].periodNumber,
               routine[i].timeSlot, routine[i].classNumber, routine[i].section, routine[i].subjectName);
    }
    printf("  ────────────────────────────────────────────────────────────────────────────────\n");
    printf(GREEN "  Total Assigned Classes: %d\n" RESET, count);

    pauseAndClear();
}
