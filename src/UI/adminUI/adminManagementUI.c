#include <stdio.h>
#include <string.h>

#include "models/user.h"
#include "utils/colors.h"
#include "utils/config.h"
#include "utils/helpers.h"
#include "ui/common/components.h"
#include "ui/adminUI/adminManagementUI.h"
#include "services/authServices/authService.h"
#include "services/adminServices/adminService.h"
#include "storage/databaseHandler.h"

static const char* getAdminTypeLabel(AdminType type) {
    switch(type) {
        case ADMIN_SUPER: return "Super Admin";
        case ADMIN_STANDARD: return "Standard Admin";
        case ADMIN_NONE: default: return "No Privilege";
    }
}

static void viewAllAdminsUI(void) {
    clearScreen();
    printf("\n  " BG_BLUE "                                                                " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "                  ADMINISTRATOR ACCOUNTS LIST                   " RESET "\n");
    printf("  " BG_BLUE "                                                                " RESET "\n\n");

    int adminCount = 0;
    printf(BOLD_WHITE "  %-6s %-20s %-20s %-15s\n" RESET, "ID", "Username", "Privilege Level", "Status");
    printf("  ────────────────────────────────────────────────────────────────\n");
    for(int i = 0; i < userCount; i++) {
        if(userDB[i].role == ROLE_ADMIN) {
            printf("  %-6d %-20.20s " RED "%-20.20s" RESET " %s\n",
                   userDB[i].id, 
                   userDB[i].username, 
                   getAdminTypeLabel(userDB[i].adminType),
                   userDB[i].isActive ? GREEN "[ACTIVE]" RESET : RED "[INACTIVE]" RESET);
            adminCount++;
        }
    }
    printf("  ────────────────────────────────────────────────────────────────\n");
    printf(GREEN "  Total Admins Registered: %d\n\n" RESET, adminCount);
    pauseAndClear();
}

static void createAdminAccountUI(void) {
    clearScreen();
    printf("\n  " BG_BLUE "                                        " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "        CREATE NEW ADMIN ACCOUNT        " RESET "\n");
    printf("  " BG_BLUE "                                        " RESET "\n\n");

    char username[MAX_STR_LEN];
    char password[MAX_STR_LEN];

    getSafeString(username, sizeof(username), "Enter admin username");
    getSafeString(password, sizeof(password), "Enter admin password");

    printf("\n  Select Admin Privilege Level:\n");
    printf("  " CYAN "1." RESET " Super Admin\n");
    printf("  " CYAN "2." RESET " Standard Admin\n");
    printf("  ────────────────────────────────────────\n");

    int choice = getSafeInt("Select Choice (1-2)", 1, 2);
    AdminType selectedType = (choice == 1) ? ADMIN_SUPER : ADMIN_STANDARD;

    int status = executeCreateAdmin(username, password, selectedType);

    if(status == 1) {
        printf("\n" GREEN "  ✔ New Admin created successfully!" RESET "\n");
    } else if(status == 0) {
        showError("This username already exists!");
    } else if(status == -1) {
        showError("User storage memory limit exceeded!");
    } else {
        showError("Database save error encountered.");
    }
    pauseAndClear();
}

static void updateAdminUsernameUI(void) {
    clearScreen();
    printf("\n  " BG_BLUE "                                                    " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "               UPDATE ADMIN USERNAME                " RESET "\n");
    printf("  " BG_BLUE "                                                    " RESET "\n\n");

    char oldUsername[MAX_STR_LEN];
    char newUsername[MAX_STR_LEN];

    getSafeString(oldUsername, sizeof(oldUsername), "Enter admin's current username");
    getSafeString(newUsername, sizeof(newUsername), "Enter admin's new username");

    int status = updateUsernameByAdmin(oldUsername, newUsername);

    if(status == 1) {
        printf("\n" GREEN "  ✔ Username changed from '%s' to '%s'!" RESET "\n", oldUsername, newUsername);
    } else if(status == -1) {
        showError("Current Admin username not found!");
    } else if(status == -2) {
        showError("This name is already taken by another user!");
    } else {
        showError("Database save error encountered.");
    }
    pauseAndClear();
}

static void updateAdminRoleUI(void) {
    clearScreen();
    printf("\n  " BG_BLUE "                                                    " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "                 UPDATE ADMIN ROLE                  " RESET "\n");
    printf("  " BG_BLUE "                                                    " RESET "\n\n");

    char targetUsername[MAX_STR_LEN];
    getSafeString(targetUsername, sizeof(targetUsername), "Enter admin's username");

    printf("\n  Select New Privilege Level for '%s':\n", targetUsername);
    printf("  " CYAN "1." RESET " Super Admin\n");
    printf("  " CYAN "2." RESET " Standard Admin\n");
    printf("  ────────────────────────────────────────\n");

    int choice = getSafeInt("Select Choice (1-2)", 1, 2);
    AdminType newType = (choice == 1) ? ADMIN_SUPER : ADMIN_STANDARD;

    int status = executeUpdateAdminRole(targetUsername, newType);

    if(status == 1) {
        printf("\n" GREEN "  ✔ Role updated successfully to '%s'!" RESET "\n", getAdminTypeLabel(newType));
    } else if(status == 0) {
        showError("Target Admin account not found!");
    } else if(status == -1) {
        showError("Specified account is NOT an Admin user.");
    } else {
        showError("Database write error encountered.");
    }
    pauseAndClear();
}

static void resetAdminPasswordUI(void) {
    clearScreen();
    printf("\n  " BG_BLUE "                                                  " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "               RESET ADMIN PASSWORD               " RESET "\n");
    printf("  " BG_BLUE "                                                  " RESET "\n\n");

    char targetUsername[MAX_STR_LEN];
    char newPassword[MAX_PASS_LEN];
    char confirmPassword[MAX_PASS_LEN];

    getSafeString(targetUsername, sizeof(targetUsername), "Enter admin's username");

    printf("  Enter new password for '%s': ", targetUsername);
    getMaskedPassword(newPassword, sizeof(newPassword));

    printf("  Confirm new password: ");
    getMaskedPassword(confirmPassword, sizeof(confirmPassword));

    if(strcmp(newPassword, confirmPassword) != 0) {
        showError("Passwords do not match!");
        pauseAndClear();
        return;
    }

    int status = resetUserPasswordByAdmin(targetUsername, newPassword);

    memset(newPassword, 0, sizeof(newPassword));
    memset(confirmPassword, 0, sizeof(confirmPassword));

    if(status == 1) {
        printf("\n" GREEN "  ✔ Password for admin '%s' updated successfully!" RESET "\n", targetUsername);
    } else if(status == -1) {
        showError("Target Admin username not found!");
    } else if(status == -3) {
        showError("Password is too weak! Use 6+ chars with Uppercase, Lowercase & Digit.");
    } else {
        showError("Database save error encountered.");
    }
    pauseAndClear();
}

static void deleteAdminAccountUI(void) {
    clearScreen();
    printf("\n  " BG_RED "                                                      " RESET "\n");
    printf("  " BG_RED BOLD_WHITE "              REMOVE / DEACTIVATE ADMIN               " RESET "\n");
    printf("  " BG_RED "                                                      " RESET "\n\n");

    char targetUsername[MAX_STR_LEN];
    getSafeString(targetUsername, sizeof(targetUsername), "Enter Admin Username to Remove");

    int status = executeDeactivateAdmin(targetUsername);

    if(status == 1) {
        printf("\n" GREEN "  ✔ Admin account '%s' has been deactivated successfully!" RESET "\n", targetUsername);
    } else if(status == -1) {
        showError("Cannot remove the primary Super Admin!");
    } else if(status == 0) {
        showError("Admin account not found!");
    } else {
        showError("Database write error encountered.");
    }
    pauseAndClear();
}

void manageAdminRolesUI(void) {
    int running = 1;
    while(running) {
        clearScreen();
        printf("\n  " BG_BLUE "                                        " RESET "\n");
        printf("  " BG_BLUE BOLD_WHITE "        ADMIN & ROLE MANAGEMENT         " RESET "\n");
        printf("  " BG_BLUE "                                        " RESET "\n\n");

        printf(CYAN "  1." RESET " View Admin Accounts\n");
        printf(CYAN "  2." RESET " Create New Admin\n");
        printf(CYAN "  3." RESET " Update Admin Username\n");
        printf(CYAN "  4." RESET " Reset Admin Password\n");
        printf(CYAN "  5." RESET " Update Admin Privilege Level\n");
        printf(CYAN "  6." RESET " Deactivate Admin Account\n");
        printf("  ────────────────────────────────────────\n");
        printf(RED "  0." RESET " Back to Admin Dashboard\n");
        printf("  ────────────────────────────────────────\n\n");

        int choice = getSafeInt("Enter choice", 0, 6);

        switch(choice) {
            case 0:
                running = 0;
                break;
            case 1:
                viewAllAdminsUI(); 
                break;
            case 2:
                createAdminAccountUI();
                break;
            case 3:
                updateAdminUsernameUI();
                break;
            case 4:
                resetAdminPasswordUI();
                break;
            case 5:
                updateAdminRoleUI();
                break;
            case 6:
                deleteAdminAccountUI();
                break;
            default:
                showError("Invalid selection!");
                pauseAndClear();
                break;
        }
    }
}
