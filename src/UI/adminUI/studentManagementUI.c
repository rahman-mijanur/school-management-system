#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/colors.h"
#include "utils/config.h"
#include "utils/helpers.h"
#include "models/student.h"
#include "models/user.h"

#include "ui/common/components.h"
#include "ui/adminUI/studentManagementUI.h"
#include "services/adminServices/studentService.h"
#include "storage/databaseHandler.h"

static void processStudentAdmissionUI(void);
static void displayAllStudentsDirectoryUI(void);
static void processStudentSearchUI(void);
static void processUpdateStudentInfoUI(void);
static void processDeleteStudentRecordUI(void);

void showStudentManagementMenu(void) {
    int running = 1;

    while(running) {
        clearScreen();
        
        printf("\n");
        printf("  " BG_BLUE "                                          " RESET "\n");
        printf("  " BG_BLUE BOLD_WHITE "            STUDENT MANAGEMENT            " RESET "\n");
        printf("  " BG_BLUE "                                          " RESET "\n");
        printf("\n");

        printf(CYAN "  1." RESET " New Student Admission\n");
        printf(CYAN "  2." RESET " View Student Directory\n");
        printf(CYAN "  3." RESET " Search Student Record\n");
        printf(CYAN "  4." RESET " Edit Student Details\n");
        printf(CYAN "  5." RESET " Delete Student Record\n");
        printf(RED  "  0." RESET " Back to Admin Dashboard\n");
        printf("  ─────────────────────────────────────────\n");

        int choice = getChoice();

        switch(choice) {
            case 1:
                processStudentAdmissionUI();
                break;
            case 2:
                displayAllStudentsDirectoryUI();
                break;
            case 3:
                processStudentSearchUI();
                break;
            case 4:
                processUpdateStudentInfoUI();
                break;
            case 5:
                processDeleteStudentRecordUI();
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

static void processStudentAdmissionUI(void) {
    clearScreen();
    
    printf("\n");
    printf("  " BG_CYAN "                                                  " RESET "\n");
    printf("  " BG_CYAN BOLD_BLACK "                STUDENT ADMISSION                 " RESET "\n");
    printf("  " BG_CYAN "                                                  " RESET "\n");
    printf("\n");

    if(studentCount >= MAX_STUDENTS || userCount >= MAX_USERS) {
        showError("System Capacity Full! Cannot add more students.");
        pauseAndClear();
        return;
    }

    Student newStudent;
    memset(&newStudent, 0, sizeof(Student));

    getSafeString(newStudent.name, sizeof(newStudent.name), "Enter Full Name");
    if(strlen(newStudent.name) == 0) return;

    newStudent.currentClass = getSafeInt("Enter Class (1-12)", 1, 12);
    getSafeString(newStudent.section, sizeof(newStudent.section), "Enter Section");
    newStudent.rollNumber = getSafeInt("Enter Roll Number", 1, 999);
    getSafeString(newStudent.phone, sizeof(newStudent.phone), "Enter Contact Number");
    getSafeString(newStudent.address, sizeof(newStudent.address), "Enter Residential Address");

    UserAccount createdAccount;

    if(registerStudent(newStudent, &createdAccount)) {
        clearScreen();
        printf("\n" GREEN "  ✔ Student Admission Successful!" RESET "\n");
        printf("  ───────────────────────────────────────\n");
        printf(BOLD_WHITE "  Assigned Student ID : %d\n" RESET, nextStudentId - 1);
        printf(BOLD_WHITE "  Name                : %s\n" RESET, newStudent.name);
        printf(BOLD_WHITE "  Class & Section     : Class %d (%s)\n" RESET, newStudent.currentClass, newStudent.section);
        printf(BOLD_WHITE "  Roll Number         : %d\n" RESET, newStudent.rollNumber);
        printf("  ───────────────────────────────────────\n");
        printf(CYAN "  [GENERATED LOGIN CREDENTIALS]\n" RESET);
        printf("  Username            : " BOLD_WHITE "%s\n" RESET, createdAccount.username);
        printf("  Default Password    : " BOLD_WHITE "%s\n" RESET, createdAccount.password);
        printf("  ───────────────────────────────────────\n");
    } else {
        showError("Failed to save student record to persistent storage.");
    }

    pauseAndClear();
}

static void displayAllStudentsDirectoryUI(void) {
    clearScreen();

    printf("\n");
    printf("  " BG_BLUE "                                                                   " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "                          STUDENT DIRECTORY                        " RESET "\n");
    printf("  " BG_BLUE "                                                                   " RESET "\n");  
    printf("\n");
    
    if(studentCount == 0) {
        showError("No student records found in the database!");
        pauseAndClear();
        return;
    }

    printf(BOLD_WHITE "  %-8s %-24s %-8s %-10s %-8s\n" RESET,
           "ID", "Name", "Class", "Section", "Roll-No");
    printf("  ──────────────────────────────────────────────────────────────────\n");

    for(int i = 0; i < studentCount; i++) {
        printf("  %-8d %-24.24s %-8d %-10s %-8d\n",
               studentDB[i].id, 
               studentDB[i].name, 
               studentDB[i].currentClass,
               studentDB[i].section, 
               studentDB[i].rollNumber);
    }
    printf("  ──────────────────────────────────────────────────────────────────\n");

    printf(GREEN "  Total Students: %d\n" RESET, studentCount);
    pauseAndClear();
}

void displayStudentProfileCard(const Student *std) {
    if(std == NULL) return;

    clearScreen();

    char titleBuffer[60];
    snprintf(titleBuffer, sizeof(titleBuffer), "STUDENT PROFILE (ID: %d)", std->id);
    
    printf("\n");
    printf("  " BOLD_WHITE "┌────────────────────────────────────────────────────────┐" RESET "\n");
    printf("  " BOLD_WHITE "│                                                        │" RESET "\n");
    printf("  " BOLD_WHITE "│                " CYAN "%-40s" RESET BOLD_WHITE "│" RESET "\n", titleBuffer);
    printf("  " BOLD_WHITE "│                                                        │" RESET "\n");
    printf("  " BOLD_WHITE "├────────────────────────────────────────────────────────┤" RESET "\n");
    printf("  " BOLD_WHITE "│                                                        │" RESET "\n");
    printf("  " BOLD_WHITE "│" RESET "  %-15s : %-35s " BOLD_WHITE "│" RESET "\n", "Name", std->name);
    printf("  " BOLD_WHITE "│" RESET "  %-15s : %-35d " BOLD_WHITE "│" RESET "\n", "Class", std->currentClass);
    printf("  " BOLD_WHITE "│" RESET "  %-15s : %-35s " BOLD_WHITE "│" RESET "\n", "Section", std->section);
    printf("  " BOLD_WHITE "│" RESET "  %-15s : %-35d " BOLD_WHITE "│" RESET "\n", "Roll No", std->rollNumber);
    printf("  " BOLD_WHITE "│" RESET "  %-15s : %-35s " BOLD_WHITE "│" RESET "\n", "Contact No", std->phone);
    printf("  " BOLD_WHITE "│" RESET "  %-15s : %-35s " BOLD_WHITE "│" RESET "\n", "Address", std->address);
    printf("  " BOLD_WHITE "│                                                        │" RESET "\n");
    printf("  " BOLD_WHITE "└────────────────────────────────────────────────────────┘" RESET "\n");

    pauseAndClear();
}

static void processStudentSearchUI(void) {
    if(studentCount == 0) {
        showError("No student records available to search!");
        pauseAndClear();
        return;
    }

    int running = 1;
    while(running) {
        clearScreen();

        printf("\n");
        printf("  " BG_BLUE "                                        " RESET "\n");
        printf("  " BG_BLUE BOLD_WHITE "             SEARCH STUDENT             " RESET "\n");
        printf("  " BG_BLUE "                                        " RESET "\n");
        printf("\n");

        printf(CYAN "  1." RESET " Search by Student ID\n");
        printf(CYAN "  2." RESET " Search by Name (Partial Match)\n");
        printf(RED "  0." RESET " Back to Student Menu\n");
        printf("  ────────────────────────────────────────\n");

        int choice = getSafeInt("Enter search method", 0, 2);

        switch(choice) {
            case 1: {
                int searchId = getSafeInt("Enter Student ID", 1, 999999);
                Student *std = findStudentById(searchId);

                if(std != NULL) {
                    displayStudentProfileCard(std);
                } else {
                    showError("Student with this ID does not exist!");
                    pauseAndClear();
                }
                break;
            }
            case 2: {
                char query[MAX_STR_LEN];
                getSafeString(query, sizeof(query), "Enter Student Name");

                Student *matches[MAX_STUDENTS];
                int count = findStudentsByName(query, matches, MAX_STUDENTS);

                if(count > 0) {
                    displayStudentProfileCard(matches[0]);
                } else {
                    showError("No matching student names found!");
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

static void processUpdateStudentInfoUI(void) {
    clearScreen();

    printf("\n");
    printf("  " BG_BLUE "                                        " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "           UPDATE STUDENT INFO          " RESET "\n");
    printf("  " BG_BLUE "                                        " RESET "\n");
    printf("\n");

    if(studentCount == 0) {
        showError("No student records found to update!");
        pauseAndClear();
        return;
    }

    int searchId = getSafeInt("Enter Student ID to update", 1, 999999);
    Student *targetStudent = findStudentById(searchId);

    if(targetStudent == NULL) {
        showError("Student with this ID does not exist!");
        pauseAndClear();
        return;
    }

    Student updatedStudent = *targetStudent;
    int editing = 1;

    while(editing) {
        clearScreen();

        printf("\n");
        printf("  " BG_BLUE "                                        " RESET "\n");
        printf("  " BG_BLUE BOLD_WHITE "           UPDATE STUDENT INFO          " RESET "\n");
        printf("  " BG_BLUE "                                        " RESET "\n");
        printf("\n");
        printf("   Profile: " BOLD_WHITE "%s (ID: %d)" RESET "\n\n", updatedStudent.name, updatedStudent.id);

        printf(CYAN "  1." RESET " Edit Name\n");
        printf(CYAN "  2." RESET " Edit Section\n");
        printf(CYAN "  3." RESET " Edit Contact Number\n");
        printf(CYAN "  4." RESET " Edit Address\n");
        printf("  ────────────────────────────────────────\n");
        printf(GREEN "  5." RESET " Save & Apply Changes\n");
        printf(RED   "  0." RESET " Cancel & Revert\n");
        printf("  ────────────────────────────────────────\n");

        int choice = getChoice();

        switch(choice) {
            case 1:
                getSafeString(updatedStudent.name, sizeof(updatedStudent.name), "Enter New Name");
                break;
            case 2:
                getSafeString(updatedStudent.section, sizeof(updatedStudent.section), "Enter New Section");
                break;
            case 3:
                getSafeString(updatedStudent.phone, sizeof(updatedStudent.phone), "Enter New Phone Number");
                break;
            case 4:
                getSafeString(updatedStudent.address, sizeof(updatedStudent.address), "Enter New Address");
                break;
            case 5:
                if(updateStudent(updatedStudent.id, updatedStudent)) {
                    printf("\n" GREEN "  ✔ Student profile updated and saved!" RESET "\n");
                } else {
                    showError("Failed to update student in database.");
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

static void processDeleteStudentRecordUI(void) {
    clearScreen();

    printf("\n");
    printf("  " BG_RED "                                                  " RESET "\n");
    printf("  " BG_RED BOLD_WHITE "               DELETE STUDENT RECORD              " RESET "\n");
    printf("  " BG_RED "                                                  " RESET "\n");
    printf("\n");

    if(studentCount == 0) {
        showError("No student records found to delete!");
        pauseAndClear();
        return;
    }

    int searchId = getSafeInt("Enter Student ID to delete", 1, 999999);
    Student *targetStudent = findStudentById(searchId);

    if(targetStudent == NULL) {
        showError("Student ID does not exist!");
        pauseAndClear();
        return;
    }

    printf("\n  " BOLD_RED "WARNING: This action is permanent!" RESET "\n\n");
    int confirm = getSafeInt("Type '1' to confirm deletion, '0' to cancel", 0, 1);

    if(confirm == 0) {
        printf("\n" GREEN "  ✔ Deletion canceled. Student record is safe." RESET "\n");
        pauseAndClear();
        return;
    }

    bool userAccountRemoved = false;
    if(deleteStudent(searchId, &userAccountRemoved)) {
        printf("\n" GREEN "  ✔ Student record deleted successfully!" RESET "\n");
        if(userAccountRemoved) {
            printf(CYAN "  Associated LMS login account was also deleted.\n" RESET);
        }
    } else {
        showError("Failed to process deletion in database file.");
    }

    pauseAndClear();
}
