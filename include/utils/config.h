#ifndef CONFIG_H
#define CONFIG_H

/*----- Input & Buffer Sizes -----*/
#define MAX_BUFFER 256
#define MAX_STR_LEN 50
#define MAX_PASS_LEN 30
#define MAX_PHONE_LEN 16
#define MAX_DATE_LEN 11

/*----- System Maximum Limits -----*/
#define MAX_USERS 600
#define MAX_STUDENTS 500
#define MAX_TEACHERS 50
#define MAX_SUBJECTS 12
#define MAX_RESULTS 5000
#define MAX_ROUTINES 100
#define MAX_EXAMS 100
#define MAX_ATTENDANCE 120
#define MAX_FEES 120
#define MAX_DAYS 6

/*----- Keys & Character Constants -----*/
#define BACKSPACE_KEY 8
#define DELETE_KEY 127
#define ENTER_KEY 13
#define NEWLINE_KEY 10
#define MIN_PRINTABLE_CHAR 32
#define MAX_PRINTABLE_CHAR 126

/*----- Database File Paths -----*/
#define USER_DATA_FILE "data/users.dat"
#define STUDENT_DATA_FILE "data/students.dat"
#define TEACHER_DATA_FILE "data/teachers.dat"
#define RESULT_DATA_FILE "data/results.dat"
#define EXAM_DATA_FILE "data/exams.dat"
#define ROUTINE_DATA_FILE "data/routines.dat"
#define ATTENDANCE_DATA_FILE "data/attendances.dat"
#define FEE_DATA_FILE "data/fees.dat"
#define SYSTEM_CONFIG_FILE "data/systemConfig.dat"
#define BACKUP_DIR "data/backups/"
#define EXPORT_DIR "data/exports/"

#endif
