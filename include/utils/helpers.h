#ifndef HELPERS_H
#define HELPERS_H

int getChoice(void);
void clearScreen(void);
void pauseAndClear(void);
void showError(const char *msg);
void showSuccess(const char *msg);
void showTransition(const char *msg);
void getMaskedPassword(char *password, int maxLen);
int getSafeInt(const char *prompt, int min, int max);
float getSafeFloat(const char *prompt, float min, float max);
void getSafeString(char *output, int maxLen, const char *prompt);
void enableVirtualTerminalProcessing(void);

#endif
