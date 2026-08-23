#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "models/user.h"
#include "utils/colors.h"
#include "utils/helpers.h"
#include "ui/common/components.h"
#include "ui/adminUI/adminDashboard.h"
#include "ui/adminUI/feeManagementUI.h"
#include "ui/adminUI/classManagementUI.h"
#include "ui/adminUI/adminManagementUI.h"
#include "ui/adminUI/systemManagementUI.h"
#include "ui/adminUI/studentManagementUI.h"
#include "ui/adminUI/teacherManagementUI.h"
#include "ui/adminUI/routineManagementUI.h"
#include "services/authServices/authService.h"

void showAdminDashboard(void) {
    int running = 1;

    if(currentUser == NULL || currentUser->role != ROLE_ADMIN) {
        showError("Unauthorized Access Blocked!");
        pauseAndClear();
        return;
    }

    bool isSuperAdmin = (currentUser->adminType == ADMIN_SUPER);

    while(running) {
        clearScreen();

        printf("\n");
        printf("  " BG_BLUE "                                               " RESET "\n");
        printf("  " BG_BLUE BOLD_WHITE "            ADMINISTRATOR DASHBOARD            " RESET "\n");
        printf("  " BG_BLUE "                                               " RESET "\n");
        printf("\n");

        printf(GREEN "  User: %s " RESET BOLD_WHITE "[Role: %s]" RESET "\n\n", currentUser->username, isSuperAdmin ? "Super Admin" : "System Admin");

        printf(CYAN "  1." RESET " Student Management\n");
        printf(CYAN "  2." RESET " Teacher Management\n");
        printf(CYAN "  3." RESET " Class Management\n");
        printf(CYAN "  4." RESET " Routine Management\n");
        printf(CYAN "  5." RESET " FEE COLLECTION & BILLING\n");
        
        if(isSuperAdmin) {
            printf(YELLOW "  6." RESET " System Admin & Privileges " BOLD_WHITE "(Super Admin)\n" RESET);
            printf(YELLOW "  7." RESET " System Utilities & Backup " BOLD_WHITE "(Super Admin)\n" RESET);
        } else {
            printf(DIM "  6. System Admin & Privileges [Restricted]\n" RESET);
            printf(DIM "  7. System Utilities & Backup [Restricted]\n" RESET);
        }

        printf(RED "  0." RESET " Logout\n");
        printf("  ───────────────────────────────────────────────\n");

        int choice = getChoice();

        switch(choice) {
            case 1:
                showStudentManagementMenu();
                break;
            case 2:
                showTeacherManagementMenu();
                break;
            case 3:
                showClassManagementMenu();
                break;
            case 4:
                manageRoutines();
                break;
            case 5:
                showFeeManagementMenu();
                break;
            case 6:
                if(isSuperAdmin) {
                    manageAdminRolesUI();
                } else {
                    showError("Access Denied! Feature reserved for Super Admin.");
                    pauseAndClear();
                }
                break;
            case 7:
                if(isSuperAdmin) {
                    displaySystemSettings();
                } else {
                    showError("Access Denied! Feature reserved for Super Admin.");
                    pauseAndClear();
                }
                break;
            case 0:
                showTransition("Logging out of Admin Session");
                running = 0;
                break;
            default:
                showError("Invalid choice! Please select 0 to 7.");
                pauseAndClear();
                break;
        }
    }
}
