#include <stdlib.h>

#include "utils/helpers.h"
#include "ui/authUI/loginUI.h"
#include "ui/common/components.h"
#include "storage/databaseHandler.h"

int main(void) {
    enableVirtualTerminalProcessing();

    if(!loadDatabase()) {
        showError("Failed to initialize database engine!");
        pauseAndClear();
        return EXIT_FAILURE;
    }

    int isRunning = 1;

    while(isRunning) {
        showMainMenu();
        
        int choice = getChoice();

        switch(choice) {
            case 1:
                handleLoginMenu();
                break;
            case 2:
                showSchoolInfo();
                break;
            case 3:
                saveDatabase();
                showExitMessage();
                isRunning = 0;
                break;
            default:
                showError("Invalid choice! Please select 1-3.");
                pauseAndClear();
                break;
        }
    }

    return EXIT_SUCCESS;
}
