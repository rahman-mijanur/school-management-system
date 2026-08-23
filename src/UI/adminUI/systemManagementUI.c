#include <stdio.h>
#include <string.h>

#include "utils/colors.h"
#include "utils/config.h"
#include "utils/helpers.h"
#include "ui/common/components.h"
#include "ui/adminUI/systemManagementUI.h"
#include "services/adminServices/systemService.h"

static void executeBackupWorkflow(void);
static void executeRestoreWorkflow(void);
static void executeExcelExportWorkflow(void);

void displaySystemSettings(void) {
    int running = 1;

    while(running) {
        clearScreen();
        printf("\n  " BG_BLUE "                                        " RESET "\n");
        printf("  " BG_BLUE BOLD_WHITE "       SYSTEM UTILITIES & BACKUP        " RESET "\n");
        printf("  " BG_BLUE "                                        " RESET "\n\n");

        printf(CYAN "  1." RESET " Create Full System Backup\n");
        printf(CYAN "  2." RESET " Restore System from Backup\n");
        printf(GREEN "  3." RESET " Export Database to Excel (.csv)\n");
        printf("  ────────────────────────────────────────\n");
        printf(RED "  0." RESET " Return to Admin Dashboard\n");
        printf("  ────────────────────────────────────────\n\n");

        int choice = getSafeInt("Enter choice", 0, 4);

        switch(choice) {
            case 0:
                running = 0;
                break;
            case 1:
                executeBackupWorkflow();
                break;
            case 2:
                executeRestoreWorkflow();
                break;
            case 3:
                executeExcelExportWorkflow();
                break;
            default:
                showError("Command syntax not recognized.");
                pauseAndClear();
                break;
        }
    }
}

static void executeBackupWorkflow(void) {
    char timestamp[MAX_DATE_LEN];

    clearScreen();
    printf("\n  " BG_BLUE "                                                   " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "            GENERATE DATABASE SNAPSHOT             " RESET "\n");
    printf("  " BG_BLUE "                                                   " RESET "\n\n");

    getSafeString(timestamp, sizeof(timestamp), "Enter Snapshot tag (Format: YYYYMMDD)");

    if(strlen(timestamp) == 0) {
        showError("Snapshot tag cannot be empty.");
        pauseAndClear();
        return;
    }

    SystemResponse res = createSystemBackup(timestamp);

    if(res == SYS_SUCCESS) {
        printf("\n  " GREEN "✔ Snapshot '%s' created successfully!" RESET "\n", timestamp);
        printf("  Files safely stored in " CYAN "%s" RESET "\n", BACKUP_DIR);
    } else {
        showError("Backup pipeline failed. Ensure 'data/backups/' directory exists.");
    }
    pauseAndClear();
}

static void executeRestoreWorkflow(void) {
    char timestamp[MAX_DATE_LEN];

    clearScreen();
    printf("\n  " BG_BLUE "                                                               " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "                   SYSTEM ROLLBACK & RESTORE                   " RESET "\n");
    printf("  " BG_BLUE "                                                               " RESET "\n\n");

    getSafeString(timestamp, sizeof(timestamp), "Enter Backup Tag to Restore (Format: YYYYMMDD)");

    if(strlen(timestamp) == 0) {
        showError("Invalid tag entry.");
        pauseAndClear();
        return;
    }

    char confirm[10];
    getSafeString(confirm, sizeof(confirm), "Are you absolutely sure you want to restore? (yes/no)");

    if(strcmp(confirm, "yes") != 0) {
        printf("\n  " YELLOW "Rollback cancelled by operator." RESET "\n");
        pauseAndClear();
        return;
    }

    SystemResponse res = restoreSystemBackup(timestamp);

    if(res == SYS_SUCCESS) {
        printf("\n  " GREEN "✔ System restored to checkpoint '%s' successfully." RESET "\n", timestamp);
        printf("  Database engine successfully hot-reloaded.\n");
    } else {
        showError("Restore failed. Checkpoint file not found or corrupted.");
    }
    pauseAndClear();
}

static void executeExcelExportWorkflow(void) {
    char customPath[260];

    clearScreen();
    printf("\n  " BG_BLUE "                                                   " RESET "\n");
    printf("  " BG_BLUE BOLD_WHITE "            EXPORT DATABASE TO MS EXCEL            " RESET "\n");
    printf("  " BG_BLUE "                                                   " RESET "\n\n");

    printf("  Enter Destination Directory Path\n");
    printf("  " YELLOW "Leave empty for default project export folder" RESET "\n");

    getSafeString(customPath, sizeof(customPath), "Path");

    const char *targetDir = EXPORT_DIR;
    if(strlen(customPath) > 0) {
        targetDir = customPath;
    }

    SystemResponse res = exportDatabaseToExcel(targetDir);

    if(res == SYS_SUCCESS) {
        printf("\n  " GREEN "✔ Database exported to Excel files successfully!" RESET "\n");
        printf("  Files safely stored in: " CYAN "%s" RESET "\n", targetDir);
    } else {
        showError("Export pipeline halted.");
        showError("Please check if the directory exists and permissions are granted.");
    }
    pauseAndClear();
}
