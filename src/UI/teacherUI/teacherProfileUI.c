#include <stdio.h>
#include <string.h>

#include "utils/colors.h"
#include "utils/helpers.h"
#include "models/teacher.h"
#include "ui/common/components.h"
#include "ui/teacherUI/teacherProfileUI.h"
#include "storage/databaseHandler.h"

static Teacher* getLoggedInTeacherDetails(int teacherId) {
    for(int i = 0; i < teacherCount; i++) {
        if(teacherDB[i].id == teacherId && teacherDB[i].isActive) {
            return &teacherDB[i];
        }
    }
    return NULL;
}

void showMyProfileUI(int teacherId) {
    clearScreen();

    Teacher *t = getLoggedInTeacherDetails(teacherId);

    if(t == NULL) {
        showError("Teacher Profile details not found in database!");
        pauseAndClear();
        return;
    }

    printf("\n");
    printf("  " BG_BLUE "                                                                " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "                      TEACHER PROFILE CARD                      " RESET "\n");
    printf("  " BG_BLUE "                                                                " RESET "\n\n");

    printf("  " BOLD_WHITE "┌──────────────────────────────────────────────────────────────┐" RESET "\n");
    printf("  " BOLD_WHITE "│ " CYAN "BASIC INFORMATION                                            " BOLD_WHITE "│" RESET "\n");
    printf("  " BOLD_WHITE "├──────────────────────────────────────────────────────────────┤" RESET "\n");
    printf("  " BOLD_WHITE "│" RESET "  %-20s : %-36d " BOLD_WHITE "│" RESET "\n", "Teacher ID", t->id);
    printf("  " BOLD_WHITE "│" RESET "  %-20s : %-36s " BOLD_WHITE "│" RESET "\n", "Full Name", t->name);
    printf("  " BOLD_WHITE "│" RESET "  %-20s : %-36s " BOLD_WHITE "│" RESET "\n", "Qualification", t->qualification);
    printf("  " BOLD_WHITE "│" RESET "  %-20s : %d years%-29s " BOLD_WHITE "│" RESET "\n", "Experience", t->experience, "");
    printf("  " BOLD_WHITE "│" RESET "  %-20s : %-36s " BOLD_WHITE "│" RESET "\n", "Joining Date", t->joiningDate);
    printf("  " BOLD_WHITE "├──────────────────────────────────────────────────────────────┤" RESET "\n");
    printf("  " BOLD_WHITE "│ " CYAN "CONTACT & ADDRESS                                            " BOLD_WHITE "│" RESET "\n");
    printf("  " BOLD_WHITE "├──────────────────────────────────────────────────────────────┤" RESET "\n");
    printf("  " BOLD_WHITE "│" RESET "  %-20s : %-36s " BOLD_WHITE "│" RESET "\n", "Phone", t->phone);
    printf("  " BOLD_WHITE "│" RESET "  %-20s : %-36s " BOLD_WHITE "│" RESET "\n", "Email", t->email);
    printf("  " BOLD_WHITE "│" RESET "  %-20s : %-36s " BOLD_WHITE "│" RESET "\n", "Address", t->address);
    printf("  " BOLD_WHITE "├──────────────────────────────────────────────────────────────┤" RESET "\n");
    printf("  " BOLD_WHITE "│ " CYAN "ASSIGNED CLASSES & SUBJECTS                                  " BOLD_WHITE "│" RESET "\n");
    printf("  " BOLD_WHITE "├──────────────────────────────────────────────────────────────┤" RESET "\n");

    if(t->assignedSubjectCount == 0) {
        printf("  " BOLD_WHITE "│" RESET "  %-68s " BOLD_WHITE "│" RESET "\n", YELLOW "No active subjects assigned yet." RESET);
    } else {
        for(int i = 0; i < t->assignedSubjectCount; i++) {
            char subjDetails[128];
            snprintf(subjDetails, sizeof(subjDetails), "Class %d (%s) - %s", t->assignedSubjects[i].classNumber, t->assignedSubjects[i].section, t->assignedSubjects[i].subjectName);
            printf("  " BOLD_WHITE "│" RESET "  %d. %-56.55s " BOLD_WHITE "│" RESET "\n", i + 1, subjDetails);
        }
    }
    printf("  " BOLD_WHITE "└──────────────────────────────────────────────────────────────┘" RESET "\n");

    pauseAndClear();
}
