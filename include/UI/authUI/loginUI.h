#ifndef LOGIN_UI_H
#define LOGIN_UI_H

#include "models/user.h"

void handleLoginMenu(void);
void processRoleLogin(UserRole expectedRole, const char *roleTitle);

#endif
