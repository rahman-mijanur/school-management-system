#ifndef STUDENT_SERVICE_H
#define STUDENT_SERVICE_H

#include <stdbool.h>
#include "models/student.h"
#include "models/user.h"

bool registerStudent(Student student, UserAccount *createdUser);
Student* findStudentById(int id);
int findStudentsByName(const char *query, Student *matches[], int maxMatches);
bool updateStudent(int studentId, Student updatedData);
bool deleteStudent(int studentId, bool *userAccountDeleted);

#endif
