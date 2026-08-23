#include <stdio.h>

#include "utils/colors.h"
#include "utils/helpers.h"
#include "ui/common/components.h"

void showMainMenu(void) {
    clearScreen();

    printf("\n");
    printf("   " BG_GREEN "                                          " RESET "\n");
    printf("   " BG_GREEN BOLD_WHITE "         SCHOOL MANAGEMENT SYSTEM         " RESET "\n");
    printf("   " BG_GREEN "                                          " RESET "\n");
    printf("\n");

    printf(CYAN "   ┌────────────────────────────────────────┐\n" RESET);
    printf(CYAN "   │" RESET GREEN "  1." RESET WHITE "  Login to Portal                   " RESET CYAN "│\n" RESET);
    printf(CYAN "   │" RESET GREEN "  2." RESET WHITE "  About Our School                  " RESET CYAN "│\n" RESET);
    printf(CYAN "   │" RESET RED  "  3." RESET WHITE "  Exit Application                  " RESET CYAN "│\n" RESET);
    printf(CYAN "   └────────────────────────────────────────┘\n" RESET);
    printf("\n");

    printf(YELLOW "   ┌──" RESET MAGENTA " DEVELOPED BY " RESET YELLOW "────────────────────────┐\n" RESET);
    printf(YELLOW "   │" RESET BLUE "  •" RESET WHITE " Md. Mijanur Rahman                  " RESET YELLOW "│\n" RESET);
    printf(YELLOW "   │" RESET BLUE "  •" RESET WHITE " Md. Abid Mia                        " RESET YELLOW "│\n" RESET);
    printf(YELLOW "   │" RESET BLUE "  •" RESET WHITE " Md. Asib                            " RESET YELLOW "│\n" RESET);
    printf(YELLOW "   └────────────────────────────────────────┘\n" RESET);
    printf("\n");
}

void showExitMessage(void) {
    clearScreen();
    
    printf("\n");
    printf(BOLD_GREEN "   ╔══════════════════════════════════════════╗\n" RESET);
    printf(BOLD_GREEN "   ║                                          ║\n" RESET);
    printf(BOLD_GREEN "   ║      THANK YOU FOR USING OUR SYSTEM      ║\n" RESET);
    printf(BOLD_GREEN "   ║                                          ║\n" RESET);
    printf(BOLD_GREEN "   ║" RESET DIM "          Application Terminated          " RESET BOLD_GREEN "║\n" RESET);
    printf(BOLD_GREEN "   ║" RESET DIM "              Successfully!               " RESET BOLD_GREEN "║\n" RESET);
    printf(BOLD_GREEN "   ║                                          ║\n" RESET);
    printf(BOLD_GREEN "   ╚══════════════════════════════════════════╝\n" RESET);
    printf("\n");
}

void showSchoolInfo(void) {
    int running = 1;
    
    while(running) {
        clearScreen();

        printf("\n");
        printf("  " BG_BLUE "                                                 " RESET "\n");
        printf("  " BG_BLUE BOLD_WHITE "                 ABOUT OUR SCHOOL                " RESET "\n");
        printf("  " BG_BLUE "                                                 " RESET "\n");
        printf("\n");

        printf(BOLD_CYAN "  ┌──────" RESET MAGENTA " 🏫 CLASSROOMS " RESET BOLD_CYAN "──────────────────────────┐\n" RESET);
        printf(BOLD_CYAN "  │                                               │\n" RESET);
        printf(BOLD_CYAN "  │" RESET "  40 Total " RESET "|" CYAN " 15 Smart Rooms                    " RESET BOLD_CYAN "│\n" RESET);
        printf(BOLD_CYAN "  │                                               │\n" RESET);
        printf(BOLD_CYAN "  ├──────" RESET MAGENTA " 📚 LIBRARY " RESET BOLD_CYAN "─────────────────────────────┤\n" RESET);
        printf(BOLD_CYAN "  │                                               │\n" RESET);
        printf(BOLD_CYAN "  │" RESET "  12,000+ Books " RESET "|" CYAN " Digital Resources            " RESET BOLD_CYAN "│\n" RESET);
        printf(BOLD_CYAN "  │                                               │\n" RESET);
        printf(BOLD_CYAN "  ├──────" RESET MAGENTA " 💻 COMPUTER LABS " RESET BOLD_CYAN "───────────────────────┤\n" RESET);
        printf(BOLD_CYAN "  │                                               │\n" RESET);
        printf(BOLD_CYAN "  │" RESET "  3 Labs " RESET "|" CYAN " 120+ PCs (High-Speed Internet)      " RESET BOLD_CYAN "│\n" RESET);
        printf(BOLD_CYAN "  │                                               │\n" RESET);
        printf(BOLD_CYAN "  ├──────" RESET MAGENTA " 🚌 TRANSPORT " RESET BOLD_CYAN "───────────────────────────┤\n" RESET);
        printf(BOLD_CYAN "  │                                               │\n" RESET);
        printf(BOLD_CYAN "  │" RESET "  12 Active Buses " RESET "|" CYAN " GPS Tracking Enabled       " RESET BOLD_CYAN "│\n" RESET);
        printf(BOLD_CYAN "  │                                               │\n" RESET);
        printf(BOLD_CYAN "  └───────────────────────────────────────────────┘\n" RESET);
        printf("\n");
        printf("    " RED "0." RESET " Back to Main Menu\n" RESET);
        
        int choice = getChoice();
        if(choice == 0) {
            running = 0;
        } else {
            showError("Invalid choice!");
            pauseAndClear();
        }
    }
}

void showComingSoon(void) {
    clearScreen();

    printf("\n");
    printf(BOLD_YELLOW "  ┌───────────────────────────────────────────┐\n" RESET);
    printf(BOLD_YELLOW "  │                                           │\n" RESET);
    printf(BOLD_YELLOW "  │" RESET BOLD_GREEN "            FEATURE COMING SOON            " BOLD_YELLOW "│\n" RESET);
    printf(BOLD_YELLOW "  │                                           │\n" RESET);
    printf(BOLD_YELLOW "  ├───────────────────────────────────────────┤\n" RESET);
    printf(BOLD_YELLOW "  │                                           │\n" RESET);
    printf(BOLD_YELLOW "  │" RESET DIM "      This feature is currently under      " RESET BOLD_YELLOW "│\n" RESET);
    printf(BOLD_YELLOW "  │" RESET DIM "         development by our team.          " RESET BOLD_YELLOW "│\n" RESET);
    printf(BOLD_YELLOW "  │                                           │\n" RESET);
    printf(BOLD_YELLOW "  └───────────────────────────────────────────┘\n" RESET);
    printf("\n");

    pauseAndClear(); 
}
