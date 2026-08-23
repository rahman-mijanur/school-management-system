#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <windows.h>

#include "utils/colors.h"
#include "utils/config.h"
#include "utils/helpers.h"

int getChoice(void) {
    char buffer[MAX_BUFFER];
    int choice;

    printf("\n" BOLD_GREEN "  Enter your choice: " RESET);

    if(fgets(buffer, sizeof(buffer), stdin) == NULL) return -1;
    if(sscanf(buffer, "%d", &choice) != 1) return -1;
    
    return choice;
}

void clearScreen(void) {
    system("cls");
}

void pauseAndClear(void) {
    printf("\n" DIM "  Press any key to continue..." RESET);

    while(kbhit()) getch();
    
    getch();
    clearScreen();
}

void showError(const char *msg) {
    printf("\n" BOLD_RED "  [X] %s\n" RESET, msg);
}

void showSuccess(const char *msg) {
    printf("\n" BOLD_GREEN "  [✔] %s\n" RESET, msg);
}

void showTransition(const char *msg) {
    clearScreen();

    printf("\n  %s", msg);
    for(int i = 0; i < 3; i++) {
        printf(".");
        fflush(stdout);
        Sleep(300);
    }
    printf("\n");

    clearScreen();
}

void getMaskedPassword(char *password, int maxLen) {
    if(password == NULL || maxLen <= 0) return;

    int i = 0;

    while(kbhit()) getch();

    while(1) {
        int ch = getch();

        if(ch == 0 || ch == 224) {
            getch();
            continue;
        }

        if(ch == ENTER_KEY || ch == NEWLINE_KEY) {
            password[i] = '\0';
            break;
        } else if(ch == BACKSPACE_KEY || ch == DELETE_KEY) {
            if(i > 0) {
                i--;
                printf("\b \b");
            }
        } else if(ch >= MIN_PRINTABLE_CHAR && ch <= MAX_PRINTABLE_CHAR && i < maxLen - 1) {
            password[i++] = (char)ch;
            printf("*");
        }
    }
    printf("\n");
}

int getSafeInt(const char *prompt, int min, int max) {
    char buffer[MAX_BUFFER];
    int value;
    char junk;

    while(1) {
        printf("  %s: ", prompt);

        if(fgets(buffer, sizeof(buffer), stdin) == NULL) continue;

        if(strchr(buffer, '\n') == NULL) {
            int c;
            while((c = getchar()) != '\n' && c != EOF);
        }

        if(sscanf(buffer, "%d %c", &value, &junk) == 1) {
            if(value >= min && value <= max) return value;
        }
        showError("Invalid number or out of range. Please try again.");
    }
}

float getSafeFloat(const char *prompt, float min, float max) {
    char buffer[MAX_BUFFER];
    float value;
    char junk;

    while(1) {
        printf("  %s: ", prompt);

        if(fgets(buffer, sizeof(buffer), stdin) == NULL) continue;

        if(strchr(buffer, '\n') == NULL) {
            int c;
            while((c = getchar()) != '\n' && c != EOF);
        }

        if(sscanf(buffer, "%f %c", &value, &junk) == 1) {
            if(value >= min && value <= max) return value;
        }
        showError("Invalid float or out of range. Please try again.");
    }
}

void getSafeString(char *output, int maxLen, const char *prompt) {
    char temp[MAX_BUFFER];

    if(prompt != NULL && strlen(prompt) > 0) {
        printf("  %s: ", prompt);
    }

    if(fgets(temp, sizeof(temp), stdin) != NULL) {
        if(strchr(temp, '\n') == NULL) {
            int c;
            while((c = getchar()) != '\n' && c != EOF);
        } else {
            temp[strcspn(temp, "\r\n")] = '\0';
        }

        strncpy(output, temp, maxLen - 1);
        output[maxLen - 1] = '\0';
    }
}

void enableVirtualTerminalProcessing(void) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if(hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if(!GetConsoleMode(hOut, &dwMode)) return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
