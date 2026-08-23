#ifndef SYSTEM_SERVICE_H
#define SYSTEM_SERVICE_H

#include <stdbool.h>

typedef enum {
    SYS_SUCCESS = 1,
    SYS_ERR_BACKUP_FAILED = 0,
    SYS_ERR_RESTORE_FAILED = -1,
    SYS_ERR_EXPORT_FAILED = -2
} SystemResponse;

SystemResponse createSystemBackup(const char *timestamp);
SystemResponse restoreSystemBackup(const char *backupDate);
bool verifyDatabaseIntegrity(void);
SystemResponse exportDatabaseToExcel(const char *destinationDir);

#endif
