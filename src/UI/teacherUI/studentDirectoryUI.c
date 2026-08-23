#include <stdio.h>
#include <string.h>

#include "utils/colors.h"
#include "utils/config.h"
#include "utils/helpers.h"
#include "models/student.h"
#include "ui/common/components.h"
#include "ui/teacherUI/studentDirectoryUI.h"
#include "services/teacherServices/studentDirectoryService.h"

static void displayStudentProfileCard(const Student *std) {
    if(std == NULL) return;

    clearScreen();
    printf("\n");
    printf("  " BOLD_WHITE "┌────────────────────────────────────────────────────────┐" RESET "\n");
    printf("  " BOLD_WHITE "│ " CYAN "STUDENT PROFILE DETAILS (ID: %-8d)                 " BOLD_WHITE "│" RESET "\n", std->id);
    printf("  " BOLD_WHITE "├────────────────────────────────────────────────────────┤" RESET "\n");
    printf("  " BOLD_WHITE "│" RESET "  %-15s : %-35s " BOLD_WHITE "│" RESET "\n", "Full Name", std->name);
    printf("  " BOLD_WHITE "│" RESET "  %-15s : Class %-29d " BOLD_WHITE "│" RESET "\n", "Academic Class", std->currentClass);
    printf("  " BOLD_WHITE "│" RESET "  %-15s : %-35s " BOLD_WHITE "│" RESET "\n", "Section", std->section);
    printf("  " BOLD_WHITE "│" RESET "  %-15s : %-35d " BOLD_WHITE "│" RESET "\n", "Roll Number", std->rollNumber);
    printf("  " BOLD_WHITE "│" RESET "  %-15s : %-35s " BOLD_WHITE "│" RESET "\n", "Contact Phone", std->phone);
    printf("  " BOLD_WHITE "│" RESET "  %-15s : %-35s " BOLD_WHITE "│" RESET "\n", "Address", std->address);
    printf("  " BOLD_WHITE "└────────────────────────────────────────────────────────┘" RESET "\n\n");

    pauseAndClear();
}

static void viewAllClassStudentsUI(int teacherId) {
    clearScreen();
    printf("\n");
    printf("  " BG_BLUE "                                                          " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "                  MY STUDENTS DIRECTORY                   " RESET "\n");
    printf("  " BG_BLUE "                                                          " RESET "\n\n");

    Student *roster[MAX_STUDENTS];
    int count = getTeacherStudentDirectory(teacherId, roster, MAX_STUDENTS);

    if(count == 0) {
        showError("No students found assigned to your active classes!");
        pauseAndClear();
        return;
    }

    printf(BOLD_WHITE "  %-10s %-18s %-8s %-8s %-8s\n" RESET, "ID", "Name", "Class", "Section", "Roll");
    printf("  ─────────────────────────────────────────────────────────\n");

    for(int i = 0; i < count; i++) {
        printf("  %-10d %-18s %-8d %-8s %-8d\n", roster[i]->id, roster[i]->name, roster[i]->currentClass, roster[i]->section, roster[i]->rollNumber);
    }
    printf("  ─────────────────────────────────────────────────────────\n");
    printf(GREEN "  Total Enrolled Students: %d\n" RESET, count);

    pauseAndClear();
}

static void searchStudentDirectoryUI(int teacherId) {
    int searching = 1;

    while(searching) {
        clearScreen();
        printf("\n");
        printf("  " BG_BLUE "                                        " RESET "\n");
        printf("  " BG_BLUE BOLD_WHITE "        SEARCH STUDENT DIRECTORY        " RESET "\n");
        printf("  " BG_BLUE "                                        " RESET "\n\n");

        printf(CYAN "  1." RESET " Search by Student ID\n");
        printf(CYAN "  2." RESET " Search by Student Name\n");
        printf(RED "  0." RESET " Back to Directory Menu\n");
        printf("  ────────────────────────────────────────\n");

        int choice = getChoice();

        switch(choice) {
            case 1: {
                int searchId = getSafeInt("Enter Student ID", 1, 999999);
                Student *std = findStudentInDirectoryById(teacherId, searchId);

                if(std != NULL) {
                    displayStudentProfileCard(std);
                } else {
                    showError("No student found with this ID!");
                    pauseAndClear();
                }
                break;
            }
            case 2: {
                char query[MAX_STR_LEN];
                getSafeString(query, sizeof(query), "Enter Student Name Query");

                Student *matches[MAX_STUDENTS];
                int count = findStudentsInDirectoryByName(teacherId, query, matches, MAX_STUDENTS);

                if(count == 1) {
                    displayStudentProfileCard(matches[0]);
                } else if(count > 1) {
                    clearScreen();
                    printf("\n" BOLD_WHITE "  Multiple Search Results Found:\n" RESET);
                    printf("  ─────────────────────────────────────────────────────────\n");
                    for(int i = 0; i < count; i++) {
                        printf("  ID: %-6d | Name: %-20s | Roll: %d\n", matches[i]->id, matches[i]->name, matches[i]->rollNumber);
                    }
                    printf("  ─────────────────────────────────────────────────────────\n\n");

                    int selId = getSafeInt("Enter Student ID to view profile", 1, 999999);
                    Student *selected = findStudentInDirectoryById(teacherId, selId);
                    if(selected != NULL) {
                        displayStudentProfileCard(selected);
                    } else {
                        showError("Invalid ID selection!");
                        pauseAndClear();
                    }
                } else {
                    showError("No matching students found!");
                    pauseAndClear();
                }
                break;
            }
            case 0:
                searching = 0;
                break;
            default:
                showError("Invalid option selected!");
                pauseAndClear();
                break;
        }
    }
}

void showStudentDirectoryUI(int teacherId) {
    int running = 1;

    while(running) {
        clearScreen();
        printf("\n");
        printf("  " BG_BLUE "                                        " RESET "\n");
        printf("  " BG_BLUE BOLD_WHITE "       STUDENT DIRECTORY SYSTEM         " RESET "\n");
        printf("  " BG_BLUE "                                        " RESET "\n\n");

        printf(CYAN "  1." RESET " View Full Student Directory\n");
        printf(CYAN "  2." RESET " Search Student Profile\n");
        printf(RED "  0." RESET " Return to Teacher Dashboard\n");
        printf("  ────────────────────────────────────────\n\n");

        int choice = getChoice();

        switch(choice) {
            case 1:
                viewAllClassStudentsUI(teacherId);
                break;
            case 2:
                searchStudentDirectoryUI(teacherId);
                break;
            case 0:
                running = 0;
                break;
            default:
                showError("Invalid selection! Try again.");
                pauseAndClear();
                break;
        }
    }
}
