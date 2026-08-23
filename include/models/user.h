#ifndef USER_H
#define USER_H

#include <stdbool.h>
#include "utils/config.h"

typedef enum {
    ROLE_ADMIN = 0,
    ROLE_TEACHER = 1,
    ROLE_STUDENT = 2
} UserRole;

typedef enum {
    ADMIN_NONE = 0,
    ADMIN_SUPER = 1,
    ADMIN_STANDARD = 2
} AdminType;

typedef struct {
    int id;
    char username[MAX_STR_LEN];
    char password[MAX_PASS_LEN];
    UserRole role;
    AdminType adminType;
    int linkedId;
    bool isActive;
} UserAccount;

#endif
