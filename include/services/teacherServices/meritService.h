#ifndef MERIT_SERVICE_H
#define MERIT_SERVICE_H

#include "models/academic.h"

int generateClassMeritList(int classNumber, MeritList *meritList, int maxStudents);
int generateSchoolMeritList(MeritList *meritList, int maxStudents);

#endif
