#ifndef TEACHER_SERVICE_H
#define TEACHER_SERVICE_H

#include <stdbool.h>
#include "models/teacher.h"
#include "models/user.h"

bool registerTeacher(Teacher teacher, UserAccount *createdUser);
Teacher* searchTeacherById(int id);
int searchTeachersByName(const char *query, Teacher *matches[], int maxMatches);
bool updateTeacher(int teacherId, Teacher updatedData);
bool deleteTeacher(int teacherId, bool *userDeletedOut);

#endif
