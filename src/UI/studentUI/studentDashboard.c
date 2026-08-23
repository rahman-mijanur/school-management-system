#include <stdio.h>
#include <string.h>

#include "models/user.h"
#include "utils/colors.h"
#include "utils/helpers.h"
#include "ui/common/components.h"
#include "ui/studentUI/studentFeeUI.h"
#include "ui/studentUI/studentExamUI.h"
#include "ui/studentUI/studentDashboard.h"
#include "ui/studentUI/studentProfileUI.h"
#include "ui/studentUI/academicReportUI.h"
#include "ui/studentUI/studentRoutineUI.h"
#include "ui/studentUI/studentAttendanceUI.h"
#include "ui/studentUI/studentAccountSettingsUI.h"
#include "services/authServices/authService.h"

void showStudentDashboard(void) {
    int running = 1;

    if(currentUser == NULL || currentUser->role != ROLE_STUDENT) {
        showError("Unauthorized Access Blocked!");
        pauseAndClear();
        return;
    }

    int studentId = currentUser->linkedId;

    while(running) {
        clearScreen();

        printf("\n");
        printf("  " BG_BLUE "                                           " RESET "\n");
        printf("  " BG_BLUE BOLD_WHITE "             STUDENT DASHBOARD             " RESET "\n");
        printf("  " BG_BLUE "                                           " RESET "\n\n");

        printf(GREEN "  User: %s " RESET BOLD_WHITE "[Student ID: %d]" RESET "\n\n", currentUser->username, studentId);

        printf(CYAN "  1." RESET " My Profile\n");
        printf(CYAN "  2." RESET " Academic Report Card\n");
        printf(CYAN "  3." RESET " Class Routine\n");
        printf(CYAN "  4." RESET " Exam Schedule\n");
        printf(CYAN "  5." RESET " Attendance Tracking\n");
        printf(CYAN "  6." RESET " Fees & Payment Ledger\n");
        printf(CYAN "  7." RESET " Account Settings\n");
        printf(RED "  0." RESET " Logout\n");
        printf("  ──────────────────────────────────────────\n");

        int choice = getChoice();

        switch(choice) {
            case 1:
                showStudentProfileUI(studentId);
                break;
            case 2:
                showAcademicReportUI(studentId);
                break;
            case 3:
                showStudentRoutineUI(studentId);
                break;
            case 4:
                showStudentExamUI(studentId);
                break;
            case 5:
                showStudentAttendanceUI(studentId);
                break;
            case 6:
                showStudentFeeUI(studentId);
                break;
            case 7:
                showStudentAccountSettingsUI();
                break;
            case 0:
                showTransition("Logging out of Student Session");
                running = 0;
                break;
            default:
                showError("Invalid choice! Please select 0 to 7.");
                pauseAndClear();
                break;
        }
    }
}
