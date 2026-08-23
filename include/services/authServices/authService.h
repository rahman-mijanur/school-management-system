#ifndef AUTH_SERVICE_H
#define AUTH_SERVICE_H

#include <stdbool.h>

#include "models/user.h"

/* Active global user session instance */
extern UserAccount *currentUser;

UserAccount* authenticateUser(const char *username, const char *password);
bool isPasswordStrong(const char *password);
int changePassword(int userId, const char *oldPassword, const char *newPassword);
int changeUsername(int userId, const char *newUsername);
int resetUserPasswordByAdmin(const char *targetUsername, const char *newPassword);
int updateUsernameByAdmin(const char *oldUsername, const char *newUsername);
void logoutUser(void);

#endif
