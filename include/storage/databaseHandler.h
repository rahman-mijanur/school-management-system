#ifndef DATABASE_HANDLER_H
#define DATABASE_HANDLER_H

#include <stdbool.h>

#include "utils/config.h"
#include "models/user.h"
#include "models/teacher.h"
#include "models/student.h"
#include "models/academic.h"
#include "models/schoolFee.h"

/* Global Storage Declarations */
extern UserAccount userDB[MAX_USERS];
extern int userCount;

extern Student studentDB[MAX_STUDENTS];
extern int studentCount;

extern Teacher teacherDB[MAX_TEACHERS];
extern int teacherCount;

extern Result resultDB[MAX_RESULTS];
extern int resultCount;

extern Exam examDB[MAX_ROUTINES];
extern int examCount;

extern Routine routineDB[MAX_ROUTINES];
extern int routineCount;

extern Attendance attendanceDB[MAX_STUDENTS * 30];
extern int attendanceCount;

extern SchoolFee feeDB[MAX_STUDENTS];
extern int feeCount;

/* Auto-Increment ID Trackers */
extern int nextStudentId;
extern int nextTeacherId;
extern int nextResultId;
extern int nextExamId;
extern int nextRoutineId;
extern int nextAttendanceId;
extern int nextFeeId;

/* Database Engine Core Functions */
bool loadDatabase(void);
bool saveDatabase(void);

#endif
