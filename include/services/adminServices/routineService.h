#ifndef ROUTINE_SERVICE_H
#define ROUTINE_SERVICE_H

#include <stdbool.h>
#include "models/academic.h"

bool isClassBusy(Routine entry);
bool isTeacherBusy(Routine entry);
bool addRoutineEntry(Routine entry);
bool deleteRoutineEntry(int routineId);

int getRoutineByClass(int classNum, const char *section, Routine matches[], int maxBuffer);
int getRoutineByTeacher(int teacherId, Routine matches[], int maxBuffer);
int getAllRoutines(Routine matches[], int maxBuffer);
const char* getTeacherNameById(int teacherId);

#endif
