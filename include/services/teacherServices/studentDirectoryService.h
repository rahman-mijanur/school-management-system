#ifndef STUDENT_DIRECTORY_SERVICE_H
#define STUDENT_DIRECTORY_SERVICE_H

#include "models/student.h"

int getTeacherStudentDirectory(int teacherId, Student *roster[], int maxStudents);
Student* findStudentInDirectoryById(int teacherId, int studentId);
int findStudentsInDirectoryByName(int teacherId, const char *query, Student *matches[], int maxMatches);

#endif
