#ifndef CLASS_SERVICE_H
#define CLASS_SERVICE_H

#include "models/academic.h"

int getClassList(ClassSummary classList[], int maxClasses);
int getStudentsInClass(int classNum, int studentIndices[], int maxBuffer);
int getTeachersForSubject(const char *subjectName, int teacherIndices[], int maxBuffer);

#endif
