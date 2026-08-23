#include <stdio.h>
#include <string.h>

#include "models/user.h"
#include "utils/colors.h"
#include "utils/config.h"
#include "utils/helpers.h"
#include "services/authServices/authService.h"
#include "ui/common/components.h"
#include "ui/studentUI/studentAccountSettingsUI.h"

static void handleChangeUsernameUI(void) {
    clearScreen();
    printf("\n");
    printf("  " BG_BLUE "                                            " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "              CHANGE USERNAME               " RESET "\n");
    printf("  " BG_BLUE "                                            " RESET "\n\n");

    if(currentUser == NULL) {
        showError("Session expired or user not logged in!");
        pauseAndClear();
        return;
    }

    printf("  Current Username: " CYAN "%s" RESET "\n\n", currentUser->username);

    char newUsername[MAX_STR_LEN];
    getSafeString(newUsername, sizeof(newUsername), "Enter New Username");

    if(strlen(newUsername) == 0) {
        showError("Username cannot be empty!");
        pauseAndClear();
        return;
    }

    if(strcmp(currentUser->username, newUsername) == 0) {
        showError("New username cannot be the same as current username!");
        pauseAndClear();
        return;
    }

    int result = changeUsername(currentUser->id, newUsername);

    if(result == 1) {
        showSuccess("Username updated successfully!");
    } else if(result == -2) {
        showError("Username is already taken by another account!");
    } else {
        showError("Failed to update username. Try again!");
    }

    pauseAndClear();
}

static void handleChangePasswordUI(void) {
    clearScreen();
    printf("\n");
    printf("  " BG_BLUE "                                              " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "               CHANGE PASSWORD                " RESET "\n");
    printf("  " BG_BLUE "                                              " RESET "\n\n");

    if(currentUser == NULL) {
        showError("Session expired or user not logged in!");
        pauseAndClear();
        return;
    }

    char oldPassword[MAX_PASS_LEN];
    char newPassword[MAX_PASS_LEN];
    char confirmPassword[MAX_PASS_LEN];

    printf("  Enter Current Password: ");
    getMaskedPassword(oldPassword, sizeof(oldPassword));

    if(strlen(oldPassword) == 0) {
        showError("Current password cannot be empty!");
        pauseAndClear();
        return;
    }

    printf("  Enter New Password (Min 6 chars, A-Z, a-z, 0-9): ");
    getMaskedPassword(newPassword, sizeof(newPassword));

    if(strlen(newPassword) == 0) {
        showError("New password cannot be empty!");
        memset(oldPassword, 0, sizeof(oldPassword));
        pauseAndClear();
        return;
    }

    if(strcmp(newPassword, oldPassword) == 0) {
        showError("New password cannot be the same as current password!");
        memset(oldPassword, 0, sizeof(oldPassword));
        memset(newPassword, 0, sizeof(newPassword));
        pauseAndClear();
        return;
    }

    printf("  Confirm New Password: ");
    getMaskedPassword(confirmPassword, sizeof(confirmPassword));

    if(strcmp(newPassword, confirmPassword) != 0) {
        showError("New password and confirm password do not match!");
        memset(oldPassword, 0, sizeof(oldPassword));
        memset(newPassword, 0, sizeof(newPassword));
        memset(confirmPassword, 0, sizeof(confirmPassword));
        pauseAndClear();
        return;
    }

    /* Backend Password Update Call */
    int result = changePassword(currentUser->id, oldPassword, newPassword);

    /* Security: Clear sensitive password buffers from RAM */
    memset(oldPassword, 0, sizeof(oldPassword));
    memset(newPassword, 0, sizeof(newPassword));
    memset(confirmPassword, 0, sizeof(confirmPassword));

    if(result == 1) {
        showSuccess("Password changed successfully!");
    } else if(result == -2) {
        showError("Current password entered is incorrect!");
    } else if(result == -3) {
        showError("Password does not meet security criteria! (Must include uppercase, lowercase, and digits)");
    } else {
        showError("Failed to change password. Try again!");
    }

    pauseAndClear();
}

void showStudentAccountSettingsUI(void) {
    int running = 1;

    while(running) {
        clearScreen();

        printf("\n");
        printf("  " BG_BLUE "                                        " RESET "\n");
        printf("  " BG_BLUE BOLD_WHITE "            ACCOUNT SETTINGS            " RESET "\n");
        printf("  " BG_BLUE "                                        " RESET "\n\n");

        printf(CYAN "  1." RESET " Change Username\n");
        printf(CYAN "  2." RESET " Change Password\n");
        printf(RED "  0." RESET " Back to Student Dashboard\n");
        printf("  ────────────────────────────────────────\n");

        int choice = getChoice();

        switch(choice) {
            case 0:
                running = 0;
                break;
            case 1:
                handleChangeUsernameUI();
                break;
            case 2:
                handleChangePasswordUI();
                break;
            default:
                showError("Invalid choice! Please select 0 to 2.");
                pauseAndClear();
                break;
        }
    }
}
