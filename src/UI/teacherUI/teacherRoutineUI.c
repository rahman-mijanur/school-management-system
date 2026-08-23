#include <stdio.h>
#include <string.h>

#include "models/academic.h"
#include "utils/colors.h"
#include "utils/helpers.h"
#include "ui/common/components.h"
#include "ui/teacherUI/teacherRoutineUI.h"
#include "services/adminServices/routineService.h"

void showTeacherRoutineUI(int teacherId) {
    clearScreen();

    printf("\n  " BG_BLUE "                                                                      " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "                           MY CLASS ROUTINE                           " RESET "\n");
    printf("  " BG_BLUE "                                                                      " RESET "\n\n");

    Routine myRoutines[MAX_ROUTINES];
    int count = getRoutineByTeacher(teacherId, myRoutines, MAX_ROUTINES);

    if(count == 0) {
        showError("No assigned class routine found for you!");
        pauseAndClear();
        return;
    }

    printf(BOLD_WHITE "  %-12s %-8s %-20s %-12s %-18s\n" RESET, "Day", "Period", "Time Slot", "Class (Sec)", "Subject");
    printf("  ──────────────────────────────────────────────────────────────────────\n");
    for(int i = 0; i < count; i++) {
        char classSec[20];
        snprintf(classSec, sizeof(classSec), "%d (%s)", myRoutines[i].classNumber, myRoutines[i].section);
        printf("  %-12s %-8d %-20s %-12s %-18s\n",
               myRoutines[i].dayOfWeek,
               myRoutines[i].periodNumber,
               myRoutines[i].timeSlot,
               classSec,
               myRoutines[i].subjectName);
    }

    printf("  ──────────────────────────────────────────────────────────────────────\n");
    printf(GREEN "  Total Assigned Classes: %d\n" RESET, count);

    pauseAndClear();
}
