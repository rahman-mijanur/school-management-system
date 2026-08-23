#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/colors.h"
#include "utils/config.h"
#include "utils/helpers.h"
#include "models/user.h"
#include "models/teacher.h"
#include "ui/common/components.h"
#include "ui/adminUI/teacherManagementUI.h"
#include "services/adminServices/teacherService.h"
#include "storage/databaseHandler.h"

static void addTeacherUI(void);
static void viewAllTeachersUI(void);
static void searchTeacherUI(void);
static void updateTeacherUI(void);
static void deleteTeacherUI(void);

void showTeacherManagementMenu(void) {
    int running = 1;

    while(running) {
        clearScreen();

        printf("\n");
        printf("  " BG_BLUE "                                          " RESET "\n");
        printf("  " BG_BLUE BOLD_WHITE "            TEACHER MANAGEMENT            " RESET "\n");
        printf("  " BG_BLUE "                                          " RESET "\n");
        printf("\n");

        printf(CYAN "  1." RESET " Add New Teacher\n");
        printf(CYAN "  2." RESET " View All Teachers\n");
        printf(CYAN "  3." RESET " Search Teacher Record\n");
        printf(CYAN "  4." RESET " Edit Teacher Details\n");
        printf(CYAN "  5." RESET " Delete Teacher Record\n");
        printf(RED  "  0." RESET " Back to Admin Dashboard\n");
        printf("  ─────────────────────────────────────────\n");

        int choice = getChoice();

        switch(choice) {
            case 1:
                addTeacherUI();
                break;
            case 2:
                viewAllTeachersUI();
                break;
            case 3:
                searchTeacherUI();
                break;
            case 4:
                updateTeacherUI();
                break;
            case 5:
                deleteTeacherUI();
                break;
            case 0:
                running = 0;
                break;
            default:
                showError("Invalid choice! Please select 0 to 5.");
                pauseAndClear();
                break;
        }
    }
}

static void addTeacherUI(void) {
    clearScreen();

    printf("\n");
    printf("  " BG_BLUE "                                                     " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "                 TEACHER RECRUITMENT                 " RESET "\n");
    printf("  " BG_BLUE "                                                     " RESET "\n");
    printf("\n");

    if(teacherCount >= MAX_TEACHERS || userCount >= MAX_USERS) {
        showError("Database limit reached! Cannot add more teachers.");
        pauseAndClear();
        return;
    }

    Teacher newTeacher;
    memset(&newTeacher, 0, sizeof(Teacher));

    getSafeString(newTeacher.name, sizeof(newTeacher.name), "Enter Full Name");
    if(strlen(newTeacher.name) == 0) return;

    getSafeString(newTeacher.qualification, sizeof(newTeacher.qualification), "Enter Qualification");
    getSafeString(newTeacher.phone, sizeof(newTeacher.phone), "Enter Contact Number");
    getSafeString(newTeacher.email, sizeof(newTeacher.email), "Enter Email Address");
    newTeacher.experience = getSafeInt("Enter Experience Years", 0, 50);
    newTeacher.salary = (double)getSafeInt("Enter Monthly Salary (BDT)", 0, 1000000);
    getSafeString(newTeacher.joiningDate, sizeof(newTeacher.joiningDate), "Enter Joining Date");

    UserAccount createdUser;

    if(registerTeacher(newTeacher, &createdUser)) {
        clearScreen();
        printf("\n" GREEN "  ✔ Teacher Registration Successful!" RESET "\n");
        printf("  ─────────────────────────────────────────\n");
        printf(BOLD_WHITE "  Assigned Teacher ID : %d\n" RESET, nextTeacherId - 1);
        printf(BOLD_WHITE "  Name                : %s\n" RESET, newTeacher.name);
        printf(BOLD_WHITE "  Qualification       : %s\n" RESET, newTeacher.qualification);
        printf(BOLD_WHITE "  Experience          : %d Years\n" RESET, newTeacher.experience);
        printf("  ─────────────────────────────────────────\n");
        printf(CYAN "  [GENERATED LOGIN CREDENTIALS]\n" RESET);
        printf("  Username            : " BOLD_WHITE "%s\n" RESET, createdUser.username);
        printf("  Default Password    : " BOLD_WHITE "%s\n" RESET, createdUser.password);
        printf("  ─────────────────────────────────────────\n");
    } else {
        showError("Failed to save teacher record to database.");
    }

    pauseAndClear();
}

static void viewAllTeachersUI(void) {
    clearScreen();

    printf("\n");
    printf("  " BG_BLUE "                                                                   " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "                          TEACHER DIRECTORY                        " RESET "\n");
    printf("  " BG_BLUE "                                                                   " RESET "\n");  
    printf("\n");
    
    if(teacherCount == 0) {
        showError("No teacher records found in the database!");
        pauseAndClear();
        return;
    }

    printf(BOLD_WHITE "  %-6s %-22s %-22s %-15s\n" RESET, "ID", "Name", "Qualification", "Phone");
    printf("  ───────────────────────────────────────────────────────────────────\n");
    
    for(int i = 0; i < teacherCount; i++) {
        printf("  %-6d %-22s %-22s %-15s\n",
               teacherDB[i].id, 
               teacherDB[i].name, 
               teacherDB[i].qualification,
               teacherDB[i].phone);
    }
    printf("  ───────────────────────────────────────────────────────────────────\n");
    printf(GREEN "  Total Enrolled Teachers: %d\n" RESET, teacherCount);

    pauseAndClear();
}

void displayTeacherProfileCard(const Teacher *tch) {
    if(tch == NULL) return;

    clearScreen();

    char titleBuffer[60];
    char salaryStr[40];
    snprintf(titleBuffer, sizeof(titleBuffer), "TEACHER PROFILE (ID: %d)", tch->id);
    snprintf(salaryStr, sizeof(salaryStr), "৳%.2f", tch->salary);
    
    printf("\n");
    printf("  " BOLD_WHITE "┌────────────────────────────────────────────────────────┐" RESET "\n");
    printf("  " BOLD_WHITE "│                                                        │" RESET "\n");
    printf("  " BOLD_WHITE "│                " CYAN "%-40s" RESET BOLD_WHITE "│" RESET "\n", titleBuffer);
    printf("  " BOLD_WHITE "│                                                        │" RESET "\n");
    printf("  " BOLD_WHITE "├────────────────────────────────────────────────────────┤" RESET "\n");
    printf("  " BOLD_WHITE "│                                                        │" RESET "\n");
    printf("  " BOLD_WHITE "│" RESET "  %-15s : %-35s " BOLD_WHITE "│" RESET "\n", "Full Name", tch->name);
    printf("  " BOLD_WHITE "│" RESET "  %-15s : %-35s " BOLD_WHITE "│" RESET "\n", "Qualification", tch->qualification);
    printf("  " BOLD_WHITE "│" RESET "  %-15s : %-35s " BOLD_WHITE "│" RESET "\n", "Contact Phone", tch->phone);
    printf("  " BOLD_WHITE "│" RESET "  %-15s : %-35s " BOLD_WHITE "│" RESET "\n", "Email Address", tch->email);
    printf("  " BOLD_WHITE "│" RESET "  %-15s : %-35d " BOLD_WHITE "│" RESET "\n", "Experience", tch->experience);
    printf("  " BOLD_WHITE "│" RESET "  %-15s : %-35s " BOLD_WHITE "│" RESET "\n", "Joining Date", tch->joiningDate);
    printf("  " BOLD_WHITE "│                                                        │" RESET "\n");
    printf("  " BOLD_WHITE "└────────────────────────────────────────────────────────┘" RESET "\n");

    pauseAndClear();
}

static void searchTeacherUI(void) {
    if(teacherCount == 0) {
        showError("No teacher records found to search!");
        pauseAndClear();
        return;
    }

    int running = 1;
    while(running) {
        clearScreen();

        printf("\n");
        printf("  " BG_BLUE "                                        " RESET "\n");
        printf("  " BG_BLUE BOLD_WHITE "             SEARCH TEACHER             " RESET "\n");
        printf("  " BG_BLUE "                                        " RESET "\n");
        printf("\n");

        printf(CYAN "  1." RESET " Search by Teacher ID\n");
        printf(CYAN "  2." RESET " Search by Name (Partial Match)\n");
        printf(RED "  0." RESET " Back to Teacher Menu\n");
        printf("  ────────────────────────────────────────\n");

        int choice = getSafeInt("Enter search method", 0, 2);

        switch(choice) {
            case 1: {
                int searchId = getSafeInt("Enter Teacher ID", 1, 999999);
                Teacher *tch = searchTeacherById(searchId);

                if(tch != NULL) {
                    displayTeacherProfileCard(tch);
                } else {
                    showError("Teacher with this ID does not exist!");
                    pauseAndClear();
                }
                break;
            }
            case 2: {
                char query[MAX_STR_LEN];
                getSafeString(query, sizeof(query), "Enter Teacher Name");

                Teacher *matches[MAX_TEACHERS];
                int count = searchTeachersByName(query, matches, MAX_TEACHERS);

                if(count > 0) {
                    displayTeacherProfileCard(matches[0]);
                } else {
                    showError("No matching teacher records found!");
                    pauseAndClear();
                }
                break;
            }
            case 0:
                running = 0;
                break;
            default:
                showError("Invalid choice! Select 0 to 2.");
                pauseAndClear();
                break;
        }
    }
}

static void updateTeacherUI(void) {
    clearScreen();

    printf("\n");
    printf("  " BG_BLUE "                                        " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "           UPDATE TEACHER INFO          " RESET "\n");
    printf("  " BG_BLUE "                                        " RESET "\n");
    printf("\n");

    if(teacherCount == 0) {
        showError("No teacher records found to update!");
        pauseAndClear();
        return;
    }

    int searchId = getSafeInt("Enter Teacher ID to edit", 1, 999999);
    Teacher *targetTeacher = searchTeacherById(searchId);

    if(targetTeacher == NULL) {
        showError("Teacher with this ID does not exist!");
        pauseAndClear();
        return;
    }

    Teacher updatedTeacher = *targetTeacher;
    int editing = 1;

    while(editing) {
        clearScreen();

        printf("\n");
        printf("  " BG_BLUE "                                        " RESET "\n");
        printf("  " BG_BLUE BOLD_WHITE "           UPDATE TEACHER INFO          " RESET "\n");
        printf("  " BG_BLUE "                                        " RESET "\n");
        printf("\n");
        printf("   Profile: " BOLD_WHITE "%s (ID: %d)" RESET "\n\n", updatedTeacher.name, updatedTeacher.id);

        printf(CYAN "  1." RESET " Edit Name\n");
        printf(CYAN "  2." RESET " Edit Qualification\n");
        printf(CYAN "  3." RESET " Edit Contact Number\n");
        printf(CYAN "  4." RESET " Edit Email Address\n");
        printf("  ────────────────────────────────────────\n");
        printf(GREEN "  5." RESET " Save & Apply Changes\n");
        printf(RED   "  0." RESET " Cancel & Revert\n");
        printf("  ────────────────────────────────────────\n");

        int choice = getChoice();

        switch(choice) {
            case 1:
                getSafeString(updatedTeacher.name, sizeof(updatedTeacher.name), "Enter New Name");
                break;
            case 2:
                getSafeString(updatedTeacher.qualification, sizeof(updatedTeacher.qualification), "Enter New Qualification");
                break;
            case 3:
                getSafeString(updatedTeacher.phone, sizeof(updatedTeacher.phone), "Enter New Phone Number");
                break;
            case 4:
                getSafeString(updatedTeacher.email, sizeof(updatedTeacher.email), "Enter New Email");
                break;
            case 5:
                if(updateTeacher(updatedTeacher.id, updatedTeacher)) {
                    printf("\n" GREEN "  ✔ Teacher profile updated and saved!" RESET "\n");
                } else {
                    showError("Failed to save updated profile to database.");
                }
                editing = 0;
                pauseAndClear();
                break;
            case 0:
                editing = 0;
                printf("\n" RED "  ✖ Update canceled. Changes discarded." RESET "\n");
                pauseAndClear();
                break;
            default:
                showError("Invalid choice!");
                pauseAndClear();
                break;
        }
    }
}

static void deleteTeacherUI(void) {
    clearScreen();

    printf("\n");
    printf("  " BG_RED "                                                  " RESET "\n");
    printf("  " BG_RED BOLD_WHITE "               DELETE TEACHER RECORD              " RESET "\n");
    printf("  " BG_RED "                                                  " RESET "\n");
    printf("\n");

    if(teacherCount == 0) {
        showError("No teacher records found to delete!");
        pauseAndClear();
        return;
    }

    int searchId = getSafeInt("Enter Teacher ID to delete", 1, 999999);
    Teacher *targetTeacher = searchTeacherById(searchId);

    if(targetTeacher == NULL) {
        showError("Teacher ID does not exist!");
        pauseAndClear();
        return;
    }

    printf("\n  " BOLD_RED "WARNING: This action is permanent!" RESET "\n\n");
    int confirm = getSafeInt("Type '1' to confirm deletion, '0' to cancel", 0, 1);

    if(confirm == 0) {
        printf("\n" GREEN "  ✔ Deletion canceled. Teacher record is safe." RESET "\n");
        pauseAndClear();
        return;
    }

    bool userRemoved = false;
    if(deleteTeacher(searchId, &userRemoved)) {
        printf("\n" GREEN "  ✔ Teacher record deleted successfully!" RESET "\n");
        if(userRemoved) {
            printf(CYAN "  Associated LMS login account was also deleted.\n" RESET);
        }
    } else {
        showError("Failed to update database after deletion.");
    }

    pauseAndClear();
}
