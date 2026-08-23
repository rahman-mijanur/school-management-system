#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/colors.h"
#include "utils/config.h"
#include "utils/helpers.h"
#include "models/student.h"
#include "models/schoolFee.h"

#include "ui/common/components.h"
#include "ui/adminUI/feeManagementUI.h"
#include "services/adminServices/feeService.h"
#include "storage/databaseHandler.h"

static int searchStudentIndexById(int id) {
    for(int i = 0; i < studentCount; i++) {
        if(studentDB[i].id == id && studentDB[i].isActive) {
            return i;
        }
    }
    return -1;
}

static int searchFeeIndexByStudentId(int studentId) {
    for(int i = 0; i < feeCount; i++) {
        if(feeDB[i].studentId == studentId && feeDB[i].isActive) {
            return i;
        }
    }
    return -1;
}

static const char* getStatusLabel(FeeStatus status) {
    switch(status) {
        case STATUS_PAID: return GREEN "[PAID]" RESET;
        case STATUS_PARTIAL: return YELLOW "[PARTIAL]" RESET;
        case STATUS_UNPAID: default: return RED "[UNPAID]" RESET;
    }
}

static void showStudentFeeCard(int studentIdx, int feeIdx) {
    printf("\n");
    printf("  " BOLD_WHITE "┌────────────────────────────────────────────────────────┐" RESET "\n");
    printf("  " BOLD_WHITE "│                                                        │" RESET "\n");
    char titleBuffer[60];
    snprintf(titleBuffer, sizeof(titleBuffer), "FEE STATUS CARD (Student ID: %d)", studentDB[studentIdx].id);
    printf("  " BOLD_WHITE "│            " CYAN "%-44s" RESET BOLD_WHITE "│" RESET "\n", titleBuffer);
    printf("  " BOLD_WHITE "│                                                        │" RESET "\n");
    printf("  " BOLD_WHITE "├────────────────────────────────────────────────────────┤" RESET "\n");
    printf("  " BOLD_WHITE "│                                                        │" RESET "\n");
    printf("  " BOLD_WHITE "│" RESET "  %-15s : %-35s " BOLD_WHITE "│" RESET "\n", "Name", studentDB[studentIdx].name);
    printf("  " BOLD_WHITE "│" RESET "  %-15s : %-35d " BOLD_WHITE "│" RESET "\n", "Class", studentDB[studentIdx].currentClass);
    printf("  " BOLD_WHITE "│" RESET "  %-15s : %-35s " BOLD_WHITE "│" RESET "\n", "Section", studentDB[studentIdx].section);
    printf("  " BOLD_WHITE "│" RESET "  %-15s : %-35d " BOLD_WHITE "│" RESET "\n", "Roll No", studentDB[studentIdx].rollNumber);
    printf("  " BOLD_WHITE "│                                                        │" RESET "\n");
    printf("  " BOLD_WHITE "├─────────────────── FINANCIAL STATUS ───────────────────┤" RESET "\n");
    printf("  " BOLD_WHITE "│                                                        │" RESET "\n");
    char totalFeeStr[40], paidFeeStr[40], dueFeeStr[40];
    if(feeIdx != -1) {
        snprintf(totalFeeStr, sizeof(totalFeeStr), "BDT %.2f", feeDB[feeIdx].feeTotal);
        snprintf(paidFeeStr, sizeof(paidFeeStr), "BDT %.2f", feeDB[feeIdx].feePaid);
        snprintf(dueFeeStr, sizeof(dueFeeStr), "BDT %.2f", feeDB[feeIdx].feeDue);
    } else {
        strcpy(totalFeeStr, "N/A");
        strcpy(paidFeeStr, "N/A");
        strcpy(dueFeeStr, "N/A");
    }
    printf("  " BOLD_WHITE "│" RESET "  %-15s : %-35s " BOLD_WHITE "│" RESET "\n", "Total Fee", totalFeeStr);
    printf("  " BOLD_WHITE "│" RESET "  %-15s : %-35s " BOLD_WHITE "│" RESET "\n", "Paid Amount", paidFeeStr);
    printf("  " BOLD_WHITE "│" RESET "  " RED "%-15s" RESET " : %-35s " BOLD_WHITE "│" RESET "\n", "Due Amount", dueFeeStr);
    printf("  " BOLD_WHITE "│" RESET "  %-15s : %-44s " BOLD_WHITE "│" RESET "\n", "Payment Status", (feeIdx != -1) ? getStatusLabel(feeDB[feeIdx].status) : RED "[NO RECORD]" RESET);
    printf("  " BOLD_WHITE "│                                                        │" RESET "\n");
    printf("  " BOLD_WHITE "└────────────────────────────────────────────────────────┘" RESET "\n\n");
}

static void collectStudentFeeUI(void) {
    clearScreen();
    printf("\n");
    printf("  " BG_BLUE "                                               " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "              COLLECT STUDENT FEE              " RESET "\n");
    printf("  " BG_BLUE "                                               " RESET "\n");
    printf("\n");

    int studentId = getSafeInt("Enter Student ID", 1, 999999);
    int sIdx = searchStudentIndexById(studentId);
    int fIdx = searchFeeIndexByStudentId(studentId);

    if(sIdx == -1 || fIdx == -1) {
        showError("Student or Fee Record with this ID does not exist!");
        pauseAndClear();
        return;
    }

    if(feeDB[fIdx].status == STATUS_PAID) {
        printf(GREEN "  Notice: This student has no outstanding dues.\n\n" RESET);
        pauseAndClear();
        return;
    }

    float amount = getSafeFloat("Enter Amount to Pay", 1.0f, feeDB[fIdx].feeDue);
    int status = executeCollectFee(studentId, amount);

    if(status == 1) {
        clearScreen();
        printf("\n");
        printf("  " BG_BLUE "                                                         " RESET "\n");
        printf("  " BG_BLUE BOLD_WHITE "                   COLLECT STUDENT FEE                   " RESET "\n");
        printf("  " BG_BLUE "                                                         " RESET "\n");
        printf("\n");
        showStudentFeeCard(sIdx, fIdx);
        printf(GREEN "  ✔ Payment processed successfully!" RESET "\n\n");
    } else {
        showError("Failed to update financial records.");
    }
    pauseAndClear();
}

static void updateClassFeeStructureUI(void) {
    clearScreen();
    printf("\n");
    printf("  " BG_BLUE "                                                   " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "            UPDATE CLASS-WISE BASE FEE             " RESET "\n");
    printf("  " BG_BLUE "                                                   " RESET "\n\n");

    int targetClass = getSafeInt("Enter Class Number (1-12)", 1, 12);
    float newClassFee = getSafeFloat("Enter New Base-Fee", 0.0f, 500000.0f);

    int count = executeClassFeeStructureUpdate(targetClass, newClassFee);

    if(count > 0) {
        printf("\n" GREEN "  ✔ Base fee updated successfully for Class %d." RESET "\n", targetClass);
    } else {
        showError("No fee records found in the specified class to update.");
    }
    pauseAndClear();
}

static void applyWaiverUI(void) {
    clearScreen();
    printf("\n");
    printf("  " BG_BLUE "                                                " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "                  APPLY WAIVER                  " RESET "\n");
    printf("  " BG_BLUE "                                                " RESET "\n");
    printf("\n");

    int studentId = getSafeInt("Enter Student ID", 1, 999999);
    int sIdx = searchStudentIndexById(studentId);
    int fIdx = searchFeeIndexByStudentId(studentId);

    if(sIdx == -1 || fIdx == -1) {
        showError("This ID is not valid!");
        pauseAndClear();
        return;
    }

    float waiver = getSafeFloat("Enter Waiver Amount (BDT)", 0.0f, feeDB[fIdx].feeTotal);
    int status = executeApplyWaiver(studentId, waiver);

    if(status == 1) {
        clearScreen();
        printf("\n");
        printf("  " BG_BLUE "                                                          " RESET "\n");
        printf("  " BG_BLUE BOLD_WHITE "                       APPLY WAIVER                       " RESET "\n");
        printf("  " BG_BLUE "                                                          " RESET "\n");
        printf("\n");

        showStudentFeeCard(sIdx, fIdx);
        printf(GREEN "  ✔ Waiver of BDT %.2f applied successfully!" RESET "\n\n", waiver);
    } else if(status == -2) {
        showError("Invalid Waiver amount! Cannot exceed total base fee.");
    } else {
        showError("Database write error encountered.");
    }
    pauseAndClear();
}

static void viewFeeDefaultersUI(void) {
    clearScreen();
    printf("\n");
    printf("  " BG_RED "                                                                                      " RESET "\n");
    printf("  " BG_RED BOLD_WHITE "                                  FEE DEFAULTERS LIST                                 " RESET "\n");
    printf("  " BG_RED "                                                                                      " RESET "\n\n");

    int defaulterCount = 0;
    for(int i = 0; i < feeCount; i++) {
        if(!feeDB[i].isActive) continue;

        if(feeDB[i].status != STATUS_PAID && feeDB[i].feeDue > 0.0f) {
            defaulterCount++;
        }
    }

    if(defaulterCount == 0) {
        printf(GREEN "  ✔ Excellent! All student fees are fully cleared. No defaulters found!\n\n" RESET);
        pauseAndClear();
        return;
    }

    printf(BOLD_WHITE "  %-10s %-15s %-8s %-12s %-12s %-12s %-10s\n" RESET, "ID", "Name", "Class", "Total Fee", "Paid", "Due", "Status");
    printf("  ─────────────────────────────────────────────────────────────────────────────────────\n");
    for(int i = 0; i < feeCount; i++) {
        if(!feeDB[i].isActive) continue;

        if(feeDB[i].status != STATUS_PAID && feeDB[i].feeDue > 0.0f) {
            int sIdx = searchStudentIndexById(feeDB[i].studentId);
            const char* name = (sIdx != -1) ? studentDB[sIdx].name : "Unknown";

            printf("  %-10d %-15s %-8d %-12.2f %-12.2f %-12.2f %s\n",
                   feeDB[i].studentId, name, feeDB[i].classNumber,
                   feeDB[i].feeTotal, feeDB[i].feePaid, feeDB[i].feeDue,
                   getStatusLabel(feeDB[i].status));
        }
    }
    printf("  ─────────────────────────────────────────────────────────────────────────────────────\n");
    printf(RED "  Total Outstanding Defaulters Found: %d\n\n" RESET, defaulterCount);

    pauseAndClear();
}

static void viewFinancialSummaryUI(void) {
    clearScreen();
    printf("\n");
    printf("  " BG_BLUE "                                           " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "         SCHOOL FINANCIAL REPORT           " RESET "\n");
    printf("  " BG_BLUE "                                           " RESET "\n\n");

    float totalExpected, totalCollected, totalDue;
    calculateFeeMetrics(&totalExpected, &totalCollected, &totalDue);

    printf("  " BOLD_WHITE "Accounts Billing Analytics" RESET "\n");
    printf("  ──────────────────────────────────────────\n");
    printf("  Total Accounts Receivables : BDT %.2f\n", totalExpected);
    printf(GREEN "  Total Fees Collected       : BDT %.2f\n" RESET, totalCollected);
    printf(RED   "  Total Outstanding Dues     : BDT %.2f\n" RESET, totalDue);
    printf("  ──────────────────────────────────────────\n");

    if(totalExpected > 0) {
        float collectionRate = (totalCollected / totalExpected) * 100.0f;
        printf("  Collection Efficiency Rate : " BOLD_WHITE "%.2f%%\n" RESET, collectionRate);
    }
    pauseAndClear();
}

void showFeeManagementMenu(void) {
    int running = 1;

    while(running) {
        clearScreen();
        printf("\n");
        printf("  " BG_BLUE "                                          " RESET "\n");
        printf("  " BG_BLUE BOLD_WHITE "         FEE COLLECTION & BILLING         " RESET "\n");
        printf("  " BG_BLUE "                                          " RESET "\n\n");

        printf(CYAN "  1." RESET " Collect Student Fee\n");
        printf(CYAN "  2." RESET " Update Class-wise Base Fee\n");
        printf(CYAN "  3." RESET " Apply Waiver to Student\n");
        printf(CYAN "  4." RESET " View Fee Defaulters List\n");
        printf(CYAN "  5." RESET " View School Financial Summary\n");
        printf(RED  "  0." RESET " Back to Admin Dashboard\n");
        printf("  ──────────────────────────────────────────\n");

        int choice = getChoice();

        switch(choice) {
            case 1:
                collectStudentFeeUI();
                break;
            case 2:
                updateClassFeeStructureUI();
                break;
            case 3:
                applyWaiverUI();
                break;
            case 4:
                viewFeeDefaultersUI();
                break;
            case 5:
                viewFinancialSummaryUI();
                break;
            case 0:
                running = 0;
                break;
            default:
                showError("Invalid selection! Select 0 to 5.");
                pauseAndClear();
                break;
        }
    }
}

void showStudentFeeCardByIdUI(int studentId) {
    int sIdx = searchStudentIndexById(studentId);
    int fIdx = searchFeeIndexByStudentId(studentId);
    if(sIdx != -1) {
        clearScreen();
        showStudentFeeCard(sIdx, fIdx);
    } else {
        showError("Student record not found!");
    }
    pauseAndClear();
}
