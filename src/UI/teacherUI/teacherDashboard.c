#include <stdio.h>
#include <string.h>

#include "models/user.h"
#include "utils/colors.h"
#include "utils/helpers.h"
#include "ui/common/components.h"
#include "ui/teacherUI/examManagementUI.h"
#include "ui/teacherUI/teacherProfileUI.h"
#include "ui/teacherUI/teacherRoutineUI.h"
#include "ui/teacherUI/teacherDashboard.h"
#include "ui/teacherUI/meritManagementUI.h"
#include "ui/teacherUI/studentDirectoryUI.h"
#include "ui/teacherUI/attendanceManagementUI.h"
#include "ui/teacherUI/teacherAccountSetingsUI.h"
#include "services/authServices/authService.h"

void showTeacherDashboard(void) {
    int running = 1;

    if(currentUser == NULL || currentUser->role != ROLE_TEACHER) {
        showError("Unauthorized Access Blocked!");
        pauseAndClear();
        return;
    }

    int teacherId = currentUser->linkedId;

    while(running) {
        clearScreen();

        printf("\n  " BG_BLUE "                                           " RESET "\n");
        printf("  " BG_BLUE BOLD_WHITE "             TEACHER DASHBOARD             " RESET "\n");
        printf("  " BG_BLUE "                                           " RESET "\n\n");

        printf(GREEN "  User: %s " RESET BOLD_WHITE "[Teacher ID: %d]" RESET "\n\n", currentUser->username, teacherId);

        printf(CYAN "  1." RESET " My Profile\n");
        printf(CYAN "  2." RESET " Student Directory\n");
        printf(CYAN "  3." RESET " Test & Exam Management\n");
        printf(CYAN "  4." RESET " Manage Attendance\n");
        printf(CYAN "  5." RESET " Manage Merit-List\n");
        printf(CYAN "  6." RESET " View Routine\n");
        printf(CYAN "  7." RESET " Account Settings\n");
        printf(RED "  0." RESET " Logout\n");
        printf("  ───────────────────────────────────────────\n");

        int choice = getChoice();

        switch(choice) {
            case 1:
                showMyProfileUI(teacherId);
                break;
            case 2:
                showStudentDirectoryUI(teacherId);
                break;
            case 3:
                manageExamsUI();
                break;
            case 4:
                manageAttendanceUI();
                break;
            case 5:
                manageMeritListUI();
                break;
            case 6:
                showTeacherRoutineUI(teacherId);
                break;
            case 7:
                showAccountSettingsUI();
                break;
            case 0:
                showTransition("Logging out of Teacher Session");
                running = 0;
                break;
            default:
                showError("Invalid choice! Please select 0 to 7.");
                pauseAndClear();
                break;
        }
    }
}
