#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>

#include "models/user.h"
#include "utils/config.h"
#include "storage/databaseHandler.h"
#include "services/adminServices/adminService.h"

bool isSuperAdminUser(const char *username) {
    if(!username) return false;
    
    for(int i = 0; i < userCount; i++) {
        if(strcasecmp(userDB[i].username, username) == 0 && userDB[i].isActive) {
            if(userDB[i].role == ROLE_ADMIN && userDB[i].adminType == ADMIN_SUPER) {
                return true;
            }
        }
    }
    return false;
}

int executeCreateAdmin(const char *username, const char *password, AdminType adminType) {
    if(!username || !password) return -2;

    for(int i = 0; i < userCount; i++) {
        if(strcasecmp(userDB[i].username, username) == 0) {
            return 0; // Username taken
        }
    }

    if(userCount >= MAX_USERS) {
        return -1; // Storage limit reached
    }

    userDB[userCount].id = userCount + 1;
    strncpy(userDB[userCount].username, username, sizeof(userDB[userCount].username) - 1);
    userDB[userCount].username[sizeof(userDB[userCount].username) - 1] = '\0';

    strncpy(userDB[userCount].password, password, sizeof(userDB[userCount].password) - 1);
    userDB[userCount].password[sizeof(userDB[userCount].password) - 1] = '\0';

    userDB[userCount].role = ROLE_ADMIN;
    userDB[userCount].adminType = adminType;
    userDB[userCount].linkedId = 0;
    userDB[userCount].isActive = true;

    userCount++;

    return saveDatabase() ? 1 : -2;
}

int executeUpdateAdminRole(const char *username, AdminType newAdminType) {
    if(!username) return 0;

    for(int i = 0; i < userCount; i++) {
        if(strcasecmp(userDB[i].username, username) == 0 && userDB[i].isActive) {
            if(userDB[i].role == ROLE_ADMIN) {
                userDB[i].adminType = newAdminType;
                return saveDatabase() ? 1 : -2;
            }
            return -1; // Not an admin account
        }
    }
    return 0; // User not found
}

int executeDeactivateAdmin(const char *username) {
    if(!username) return 0;

    for(int i = 0; i < userCount; i++) {
        if(strcasecmp(userDB[i].username, username) == 0 && userDB[i].isActive) {
            if(userDB[i].id == 1) { 
                return -1; // Protect main Super Admin
            }
            userDB[i].isActive = false;
            return saveDatabase() ? 1 : -2;
        }
    }
    return 0;
}
