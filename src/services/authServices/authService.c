#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "models/user.h"
#include "utils/config.h"
#include "storage/databaseHandler.h"
#include "services/authServices/authService.h"

/* Global Session Pointer */
UserAccount *currentUser = NULL;

bool isPasswordStrong(const char *password) {
    if(password == NULL) return false;

    size_t length = strlen(password);
    if(length < 6) return false;

    bool hasUpper = false, hasLower = false, hasDigit = false;
    for(size_t i = 0; i < length; i++) {
        if(isupper((unsigned char)password[i])) hasUpper = true;
        if(islower((unsigned char)password[i])) hasLower = true;
        if(isdigit((unsigned char)password[i])) hasDigit = true;
    }

    return (hasUpper && hasLower && hasDigit);
}

UserAccount* authenticateUser(const char *username, const char *password) {
    if(username == NULL || password == NULL) return NULL;

    for(int i = 0; i < userCount; i++) {
        if(userDB[i].isActive &&
            strcmp(userDB[i].username, username) == 0 &&
            strcmp(userDB[i].password, password) == 0) {
            
            currentUser = &userDB[i];
            return currentUser;
        }
    }

    return NULL;
}

int changePassword(int userId, const char *oldPassword, const char *newPassword) {
    if(oldPassword == NULL || newPassword == NULL) return 0;

    int userIndex = -1;
    for(int i = 0; i < userCount; i++) {
        if(userDB[i].id == userId && userDB[i].isActive) {
            userIndex = i;
            break;
        }
    }

    if(userIndex == -1) return -1; // User not found
    if(strcmp(userDB[userIndex].password, oldPassword) != 0) return -2; // Old password wrong
    if(!isPasswordStrong(newPassword)) return -3; // Weak password

    strncpy(userDB[userIndex].password, newPassword, MAX_PASS_LEN - 1);
    userDB[userIndex].password[MAX_PASS_LEN - 1] = '\0';

    return saveDatabase() ? 1 : 0;
}

int changeUsername(int userId, const char *newUsername) {
    if(newUsername == NULL || strlen(newUsername) == 0) return 0;

    // Check if the new username is already taken
    for(int i = 0; i < userCount; i++) {
        if(userDB[i].isActive && strcmp(userDB[i].username, newUsername) == 0) {
            if(userDB[i].id != userId) {
                return -2; // Username already taken
            }
        }
    }

    // Find target user and update username
    for(int i = 0; i < userCount; i++) {
        if(userDB[i].id == userId && userDB[i].isActive) {
            strncpy(userDB[i].username, newUsername, MAX_STR_LEN - 1);
            userDB[i].username[MAX_STR_LEN - 1] = '\0';
            
            return saveDatabase() ? 1 : 0;
        }
    }

    return -1; // User not found
}

int resetUserPasswordByAdmin(const char *targetUsername, const char *newPassword) {
    if(targetUsername == NULL || newPassword == NULL) return 0;

    int userIndex = -1;
    for(int i = 0; i < userCount; i++) {
        if(strcmp(userDB[i].username, targetUsername) == 0) {
            userIndex = i;
            break;
        }
    }

    if(userIndex == -1) return -1;
    if(!isPasswordStrong(newPassword)) return -3;

    strncpy(userDB[userIndex].password, newPassword, MAX_PASS_LEN - 1);
    userDB[userIndex].password[MAX_PASS_LEN - 1] = '\0';

    return saveDatabase() ? 1 : 0;
}

/* Admin direct username update logic */
int updateUsernameByAdmin(const char *oldUsername, const char *newUsername) {
    if(oldUsername == NULL || newUsername == NULL || strlen(newUsername) == 0) return 0;

    int targetIndex = -1;

    // Check if oldUsername exists and find target index
    for(int i = 0; i < userCount; i++) {
        if(strcasecmp(userDB[i].username, oldUsername) == 0 && userDB[i].isActive) {
            targetIndex = i;
            break;
        }
    }

    if(targetIndex == -1) return -1; // Target user not found

    // Check if newUsername is already taken by ANOTHER user
    for(int i = 0; i < userCount; i++) {
        if(userDB[i].isActive && strcasecmp(userDB[i].username, newUsername) == 0) {
            if(i != targetIndex) {
                return -2; // Username conflict
            }
        }
    }

    // Update Username
    strncpy(userDB[targetIndex].username, newUsername, MAX_STR_LEN - 1);
    userDB[targetIndex].username[MAX_STR_LEN - 1] = '\0';

    return saveDatabase() ? 1 : 0;
}

void logoutUser(void) {
    currentUser = NULL;
}
