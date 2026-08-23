#ifndef ATTENDANCE_SERVICE_H
#define ATTENDANCE_SERVICE_H

#include <stdbool.h>
#include "models/academic.h"

int markStudentAttendance(int studentId, int classNumber, const char *date, bool isPresent);
float calculateStudentAttendanceRate(int studentId);
int getStudentAttendanceHistory(int studentId, Attendance matches[], int maxBuffer);

#endif
