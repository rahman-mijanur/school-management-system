#include <stdio.h>
#include <string.h>

#include "models/academic.h"
#include "models/student.h"
#include "storage/databaseHandler.h"
#include "services/teacherServices/attendanceService.h"

int markStudentAttendance(int studentId, int classNumber, const char *date, bool isPresent) {
    if(studentId <= 0 || classNumber <= 0 || date == NULL) return 0;

    Attendance rec;
    rec.attendanceId = attendanceCount + 1;
    rec.studentId = studentId;
    rec.classNumber = classNumber;
    strncpy(rec.date, date, sizeof(rec.date) - 1);
    rec.date[sizeof(rec.date) - 1] = '\0';
    rec.isPresent = isPresent;

    attendanceDB[attendanceCount++] = rec;

    saveDatabase();
    return 1;
}

float calculateStudentAttendanceRate(int studentId) {
    int total = 0;
    int present = 0;

    for(int i = 0; i < attendanceCount; i++) {
        if(attendanceDB[i].studentId == studentId) {
            total++;
            if(attendanceDB[i].isPresent) {
                present++;
            }
        }
    }

    if(total == 0) return 0.0f;
    return ((float)present / total) * 100.0f;
}

int getStudentAttendanceHistory(int studentId, Attendance matches[], int maxBuffer) {
    if(studentId <= 0 || matches == NULL || maxBuffer <= 0) return 0;

    int count = 0;
    for(int i = 0; i < attendanceCount; i++) {
        if(attendanceDB[i].studentId == studentId) {
            if(count < maxBuffer) {
                matches[count++] = attendanceDB[i];
            }
        }
    }
    return count;
}
