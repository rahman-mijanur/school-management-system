#include <stdio.h>
#include <string.h>

#include "utils/colors.h"
#include "utils/helpers.h"
#include "models/student.h"
#include "models/schoolFee.h"
#include "ui/common/components.h"
#include "ui/studentUI/studentFeeUI.h"
#include "storage/databaseHandler.h"

static Student* getStudentDetails(int studentId) {
    for(int i = 0; i < studentCount; i++) {
        if(studentDB[i].id == studentId && studentDB[i].isActive) {
            return &studentDB[i];
        }
    }
    return NULL;
}

static int getStudentFeeRecords(int studentId, SchoolFee matches[], int maxMatches) {
    int count = 0;
    for(int i = 0; i < feeCount; i++) {
        if(feeDB[i].studentId == studentId && feeDB[i].isActive) {
            if(count < maxMatches) {
                matches[count++] = feeDB[i];
            }
        }
    }
    return count;
}

static const char* getStatusBadge(FeeStatus status) {
    switch(status) {
        case STATUS_PAID:
            return GREEN "PAID" RESET;
        case STATUS_PARTIAL:
            return YELLOW "PARTIAL" RESET;
        case STATUS_UNPAID:
            return RED "UNPAID" RESET;
        default:
            return "UNKNOWN";
    }
}

void showStudentFeeUI(int studentId) {
    clearScreen();

    printf("\n");
    printf("  " BG_BLUE "                                                                " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "                     FEES & PAYMENT LEDGER                      " RESET "\n");
    printf("  " BG_BLUE "                                                                " RESET "\n\n");

    Student *std = getStudentDetails(studentId);

    if(std == NULL) {
        showError("Student profile details not found!");
        pauseAndClear();
        return;
    }

    SchoolFee myFees[MAX_FEES];
    int count = getStudentFeeRecords(studentId, myFees, MAX_FEES);

    if(count == 0) {
        showError("No fee or payment ledger entries found for your account!");
        pauseAndClear();
        return;
    }

    float grandTotalFee = 0.0f;
    float grandTotalPaid = 0.0f;
    float grandTotalDue = 0.0f;

    for(int i = 0; i < count; i++) {
        grandTotalFee += myFees[i].feeTotal;
        grandTotalPaid += myFees[i].feePaid;
        grandTotalDue += myFees[i].feeDue;
    }

    printf(GREEN "  Student: %s " RESET " | " BOLD_WHITE "Class: %d (%s)" RESET "\n\n", std->name, std->currentClass, std->section);
    printf("  " BOLD_WHITE "┌──────────────────────────────────────────────────────────────┐" RESET "\n");
    printf("  " BOLD_WHITE "│ " CYAN "PAYMENT SUMMARY OVERVIEW                                     " BOLD_WHITE "│" RESET "\n");
    printf("  " BOLD_WHITE "├──────────────────────────────────────────────────────────────┤" RESET "\n");
    printf("  " BOLD_WHITE "│" RESET "  %-22s : BDT %-30.2f " BOLD_WHITE "│" RESET "\n", "Total Fee Amount", grandTotalFee);
    printf("  " BOLD_WHITE "│" RESET "  %-22s : " GREEN "BDT %-31.2f" BOLD_WHITE "│" RESET "\n", "Total Paid Amount", grandTotalPaid);
    printf("  " BOLD_WHITE "│" RESET "  %-22s : " RED "BDT %-31.2f" BOLD_WHITE "│" RESET "\n", "Total Outstanding Due", grandTotalDue);
    printf("  " BOLD_WHITE "└──────────────────────────────────────────────────────────────┘" RESET "\n\n");
    printf(BOLD_WHITE "  %-4s %-14s %-10s %-10s %-10s %-10s %-12s\n" RESET, "SL", "Fee Type", "Total", "Paid", "Due", "Status", "Last Payment");
    printf("  ─────────────────────────────────────────────────────────────────────────────\n");
    for(int i = 0; i < count; i++) {
        const char *dateStr = (strlen(myFees[i].lastPaymentDate) > 0) ? myFees[i].lastPaymentDate : "N/A";
        printf("  %-4d %-14s %-10.2f %-10.2f %-10.2f %-19s %-12s\n",
               i + 1,
               myFees[i].feeType,
               myFees[i].feeTotal,
               myFees[i].feePaid,
               myFees[i].feeDue,
               getStatusBadge(myFees[i].status),
               dateStr);
    }
    printf("  ─────────────────────────────────────────────────────────────────────────────\n");

    pauseAndClear();
}
