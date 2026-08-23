#include <stdio.h>
#include <string.h>

#include "utils/colors.h"
#include "utils/config.h"
#include "utils/helpers.h"
#include "ui/authUI/loginUI.h"
#include "ui/common/components.h"
#include "ui/adminUI/adminDashboard.h"
#include "ui/teacherUI/teacherDashboard.h"
#include "ui/studentUI/studentDashboard.h"
#include "services/authServices/authService.h"

void processRoleLogin(UserRole expectedRole, const char *roleTitle) {
    char username[MAX_STR_LEN];
    char password[MAX_PASS_LEN];

    clearScreen();

    printf("\n");
    printf("  " BG_CYAN BOLD_BLACK "                                             " RESET "\n");
    printf("  " BG_CYAN BOLD_BLACK "                %-13s                " RESET "\n", roleTitle);
    printf("  " BG_CYAN BOLD_BLACK "                                             " RESET "\n");
    printf("\n");

    printf("  " DIM "Type 'back' to return to portal selection" RESET "\n\n");

    getSafeString(username, sizeof(username), "Enter your name");

    if(strlen(username) == 0) {
        showError("Username cannot be empty!");
        pauseAndClear();
        return;
    }

    if(stricmp(username, "back") == 0) return;

    printf("  Enter your password: ");
    getMaskedPassword(password, sizeof(password));

    if(strlen(password) == 0) {
        showError("Password cannot be empty!");
        pauseAndClear();
        return;
    }

    UserAccount *user = authenticateUser(username, password);

    // Clear password from RAM buffer
    memset(password, 0, sizeof(password));

    if(user == NULL) {
        showError("Invalid username or password! Access denied.");
        pauseAndClear();
        return;
    }

    // Role verification check
    if(user->role != expectedRole) {
        showError("Unauthorized! You cannot login here with this account type.");
        logoutUser();
        pauseAndClear();
        return;
    }

    showTransition("Authenticating credentials");

    // Route based on role
    switch (user->role) {
        case ROLE_ADMIN:
            showAdminDashboard();
            break;

        case ROLE_TEACHER:
            showTeacherDashboard();
            break;

        case ROLE_STUDENT:
            showStudentDashboard();
            break;

        default:
            showError("Critical Error: Unknown user role!");
            pauseAndClear();
            break;
    }

    logoutUser(); // Clear global user session on exit
}

void handleLoginMenu(void) {
    int running = 1;

    while(running) {
        clearScreen();

        printf("\n");
        printf("  " BG_BLUE "                                        " RESET "\n");
        printf("  " BG_BLUE BOLD_WHITE "            PORTAL SELECTION            " RESET "\n");
        printf("  " BG_BLUE "                                        " RESET "\n");
        printf("\n");

        printf("  " BOLD_YELLOW "1." RESET " Admin Portal\n");
        printf("  " BOLD_YELLOW "2." RESET " Teacher Portal\n");
        printf("  " BOLD_YELLOW "3." RESET " Student Portal\n");
        printf("  " BOLD_RED    "0." RESET " Back to Main Menu\n");
        printf("  ────────────────────────────────────────\n");

        int choice = getChoice();

        switch(choice) {
            case 1:
                processRoleLogin(ROLE_ADMIN, "ADMIN LOGIN");
                break;
            case 2:
                processRoleLogin(ROLE_TEACHER, "TEACHER LOGIN");
                break;
            case 3:
                processRoleLogin(ROLE_STUDENT, "STUDENT LOGIN");
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
