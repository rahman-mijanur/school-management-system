#ifndef ADMIN_SERVICE_H
#define ADMIN_SERVICE_H

#include <stdbool.h>
#include "models/user.h"

bool isSuperAdminUser(const char *username);
int executeCreateAdmin(const char *username, const char *password, AdminType adminType);
int executeUpdateAdminRole(const char *username, AdminType newAdminType);
int executeDeactivateAdmin(const char *username);

#endif
