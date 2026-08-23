#include <stdio.h>
#include <string.h>

#include "models/student.h"
#include "utils/colors.h"
#include "utils/helpers.h"
#include "ui/common/components.h"
#include "ui/studentUI/studentProfileUI.h"
#include "storage/databaseHandler.h"

static Student* getLoggedInStudentDetails(int studentId) {
    for(int i = 0; i < studentCount; i++) {
        if(studentDB[i].id == studentId && studentDB[i].isActive) {
            return &studentDB[i];
        }
    }
    return NULL;
}

void showStudentProfileUI(int studentId) {
    clearScreen();

    Student *std = getLoggedInStudentDetails(studentId);

    if(std == NULL) {
        showError("Student Profile details not found in database!");
        pauseAndClear();
        return;
    }

    printf("\n");
    printf("  " BG_BLUE "                                                                " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "                      STUDENT PROFILE CARD                      " RESET "\n");
    printf("  " BG_BLUE "                                                                " RESET "\n\n");

    printf("  " BOLD_WHITE "┌──────────────────────────────────────────────────────────────┐" RESET "\n");
    printf("  " BOLD_WHITE "│ " CYAN "ACADEMIC INFORMATION                                         " BOLD_WHITE "│" RESET "\n");
    printf("  " BOLD_WHITE "├──────────────────────────────────────────────────────────────┤" RESET "\n");
    printf("  " BOLD_WHITE "│" RESET "  %-20s : %-36d " BOLD_WHITE "│" RESET "\n", "Student ID", std->id);
    printf("  " BOLD_WHITE "│" RESET "  %-20s : %-36s " BOLD_WHITE "│" RESET "\n", "Full Name", std->name);
    printf("  " BOLD_WHITE "│" RESET "  %-20s : %-36d " BOLD_WHITE "│" RESET "\n", "Class", std->currentClass);
    printf("  " BOLD_WHITE "│" RESET "  %-20s : %-36s " BOLD_WHITE "│" RESET "\n", "Section", std->section);
    printf("  " BOLD_WHITE "│" RESET "  %-20s : %-36d " BOLD_WHITE "│" RESET "\n", "Roll Number", std->rollNumber);
    printf("  " BOLD_WHITE "│" RESET "  %-20s : %-36d " BOLD_WHITE "│" RESET "\n", "Academic Year", std->academicYear);
    printf("  " BOLD_WHITE "│" RESET "  %-20s : %-36s " BOLD_WHITE "│" RESET "\n", "Admission Date", std->admissionDate);
    printf("  " BOLD_WHITE "├──────────────────────────────────────────────────────────────┤" RESET "\n");
    printf("  " BOLD_WHITE "│ " CYAN "PERSONAL DETAILS                                             " BOLD_WHITE "│" RESET "\n");
    printf("  " BOLD_WHITE "├──────────────────────────────────────────────────────────────┤" RESET "\n");
    printf("  " BOLD_WHITE "│" RESET "  %-20s : %-36s " BOLD_WHITE "│" RESET "\n", "Father's Name", std->fatherName);
    printf("  " BOLD_WHITE "│" RESET "  %-20s : %-36s " BOLD_WHITE "│" RESET "\n", "Mother's Name", std->motherName);
    printf("  " BOLD_WHITE "│" RESET "  %-20s : %-36s " BOLD_WHITE "│" RESET "\n", "Date of Birth", std->dateOfBirth);
    printf("  " BOLD_WHITE "│" RESET "  %-20s : %-36s " BOLD_WHITE "│" RESET "\n", "Gender", std->gender);
    printf("  " BOLD_WHITE "│" RESET "  %-20s : %-36s " BOLD_WHITE "│" RESET "\n", "Blood Group", std->bloodGroup);
    printf("  " BOLD_WHITE "├──────────────────────────────────────────────────────────────┤" RESET "\n");
    printf("  " BOLD_WHITE "│ " CYAN "CONTACT INFORMATION                                          " BOLD_WHITE "│" RESET "\n");
    printf("  " BOLD_WHITE "├──────────────────────────────────────────────────────────────┤" RESET "\n");
    printf("  " BOLD_WHITE "│" RESET "  %-20s : %-36s " BOLD_WHITE "│" RESET "\n", "Phone", std->phone);
    printf("  " BOLD_WHITE "│" RESET "  %-20s : %-36s " BOLD_WHITE "│" RESET "\n", "Email", std->email);
    printf("  " BOLD_WHITE "│" RESET "  %-20s : %-36s " BOLD_WHITE "│" RESET "\n", "Address", std->address);
    printf("  " BOLD_WHITE "└──────────────────────────────────────────────────────────────┘" RESET "\n\n");

    pauseAndClear();
}
