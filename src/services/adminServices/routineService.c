#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "utils/config.h"
#include "models/academic.h"
#include "models/teacher.h"
#include "storage/databaseHandler.h"
#include "services/adminServices/routineService.h"

const char* getTeacherNameById(int teacherId) {
    for(int i = 0; i < teacherCount; i++) {
        if(teacherDB[i].id == teacherId && teacherDB[i].isActive) {
            return teacherDB[i].name;
        }
    }
    return "Unknown Teacher";
}

bool isClassBusy(Routine entry) {
    for(int i = 0; i < routineCount; i++) {
        if(routineDB[i].classNumber == entry.classNumber &&
            strcasecmp(routineDB[i].section, entry.section) == 0 &&
            strcasecmp(routineDB[i].dayOfWeek, entry.dayOfWeek) == 0 &&
            (routineDB[i].periodNumber == entry.periodNumber || 
             strcasecmp(routineDB[i].timeSlot, entry.timeSlot) == 0)) {
            return true;
        }
    }
    return false;
}

bool isTeacherBusy(Routine entry) {
    for(int i = 0; i < routineCount; i++) {
        if(routineDB[i].teacherId == entry.teacherId &&
            strcasecmp(routineDB[i].dayOfWeek, entry.dayOfWeek) == 0 &&
            (routineDB[i].periodNumber == entry.periodNumber || 
             strcasecmp(routineDB[i].timeSlot, entry.timeSlot) == 0)) {
            return true;
        }
    }
    return false;
}

bool addRoutineEntry(Routine entry) {
    if(routineCount >= MAX_ROUTINES) return false;
    if(isClassBusy(entry) || isTeacherBusy(entry)) return false;

    entry.id = nextRoutineId++;
    routineDB[routineCount] = entry;
    routineCount++;

    if(saveDatabase()) {
        return true;
    } else {
        routineCount--;
        nextRoutineId--;
        return false;
    }
}

int getRoutineByClass(int classNum, const char *section, Routine matches[], int maxBuffer) {
    int count = 0;
    for(int i = 0; i < routineCount; i++) {
        if(routineDB[i].classNumber == classNum) {
            if(section == NULL || strlen(section) == 0 || 
                strcasecmp(routineDB[i].section, section) == 0) {
                if(count < maxBuffer) {
                    matches[count++] = routineDB[i];
                }
            }
        }
    }
    return count;
}

int getRoutineByTeacher(int teacherId, Routine matches[], int maxBuffer) {
    if(teacherId <= 0) return 0;

    int count = 0;
    for(int i = 0; i < routineCount; i++) {
        if(routineDB[i].teacherId == teacherId) {
            if(count < maxBuffer) {
                matches[count++] = routineDB[i];
            }
        }
    }
    return count;
}

int getAllRoutines(Routine matches[], int maxBuffer) {
    int count = 0;
    for(int i = 0; i < routineCount; i++) {
        if(count < maxBuffer) {
            matches[count++] = routineDB[i];
        }
    }
    return count;
}

bool deleteRoutineEntry(int routineId) {
    int targetIdx = -1;
    for(int i = 0; i < routineCount; i++) {
        if(routineDB[i].id == routineId) {
            targetIdx = i;
            break;
        }
    }

    if(targetIdx == -1) return false;

    for(int i = targetIdx; i < routineCount - 1; i++) {
        routineDB[i] = routineDB[i + 1];
    }
    routineCount--;

    return saveDatabase();
}
