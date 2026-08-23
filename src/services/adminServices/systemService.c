#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <direct.h>

#include "utils/config.h"
#include "models/user.h"
#include "models/student.h"
#include "models/teacher.h"
#include "models/academic.h"
#include "models/schoolFee.h"
#include "storage/databaseHandler.h"
#include "services/adminServices/systemService.h"

#define WIN_PATH_SEP "\\"

static int createDirectory(const char *path) {
    if(path == NULL || strlen(path) == 0) return -1;
    return _mkdir(path);
}

static void ensureBackupDirectoryExists(void) {
    createDirectory("data");
    createDirectory(BACKUP_DIR);
}

static bool copyFile(const char *source, const char *destination) {
    FILE *src = fopen(source, "rb");
    if(!src) return false;

    FILE *dst = fopen(destination, "wb");
    if(!dst) {
        fclose(src);
        return false;
    }

    char buffer[4096];
    size_t bytesRead;
    while((bytesRead = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, bytesRead, dst);
    }

    fclose(src);
    fclose(dst);
    return true;
}

static void formatExcelField(char *dest, const char *src, size_t destSize) {
    if(src == NULL) {
        dest[0] = '\0';
        return;
    }
    if(strchr(src, ',') || strchr(src, '\n') || strchr(src, '\r')) {
        snprintf(dest, destSize, "\"%s\"", src);
    } else {
        snprintf(dest, destSize, "%s", src);
    }
}

SystemResponse createSystemBackup(const char *timestamp) {
    if(timestamp == NULL || strlen(timestamp) == 0) {
        return SYS_ERR_BACKUP_FAILED;
    }

    ensureBackupDirectoryExists();

    if(!saveDatabase()) {
        return SYS_ERR_BACKUP_FAILED;
    }

    char destUser[MAX_BUFFER];
    char destStudent[MAX_BUFFER];
    char destTeacher[MAX_BUFFER];
    char destResult[MAX_BUFFER];
    char destExam[MAX_BUFFER];
    char destRoutine[MAX_BUFFER];
    char destAttendance[MAX_BUFFER];
    char destFee[MAX_BUFFER];
    char destSystemConfig[MAX_BUFFER];

    snprintf(destUser, sizeof(destUser), "%susers_%s.bak", BACKUP_DIR, timestamp);
    snprintf(destStudent, sizeof(destStudent), "%sstudents_%s.bak", BACKUP_DIR, timestamp);
    snprintf(destTeacher, sizeof(destTeacher), "%steachers_%s.bak", BACKUP_DIR, timestamp);
    snprintf(destResult, sizeof(destResult), "%sresults_%s.bak", BACKUP_DIR, timestamp);
    snprintf(destExam, sizeof(destExam), "%sexams_%s.bak", BACKUP_DIR, timestamp);
    snprintf(destRoutine, sizeof(destRoutine), "%sroutines_%s.bak", BACKUP_DIR, timestamp);
    snprintf(destAttendance, sizeof(destAttendance), "%sattendances_%s.bak", BACKUP_DIR, timestamp);
    snprintf(destFee, sizeof(destFee), "%sfees_%s.bak", BACKUP_DIR, timestamp);
    snprintf(destSystemConfig, sizeof(destSystemConfig), "%ssystemConfig_%s.bak", BACKUP_DIR, timestamp);

    bool uB   = copyFile(USER_DATA_FILE, destUser);
    bool sB   = copyFile(STUDENT_DATA_FILE, destStudent);
    bool tB   = copyFile(TEACHER_DATA_FILE, destTeacher);
    bool rB   = copyFile(RESULT_DATA_FILE, destResult);
    bool eB   = copyFile(EXAM_DATA_FILE, destExam);
    bool rtB  = copyFile(ROUTINE_DATA_FILE, destRoutine);
    bool aB   = copyFile(ATTENDANCE_DATA_FILE, destAttendance);
    bool fB   = copyFile(FEE_DATA_FILE, destFee);

    copyFile(SYSTEM_CONFIG_FILE, destSystemConfig);

    if(uB && sB && tB && rB && eB && rtB && aB && fB) {
        return SYS_SUCCESS;
    }
    return SYS_ERR_BACKUP_FAILED;
}

SystemResponse restoreSystemBackup(const char *backupDate) {
    if(backupDate == NULL || strlen(backupDate) == 0) {
        return SYS_ERR_RESTORE_FAILED;
    }

    char srcUser[MAX_BUFFER];
    char srcStudent[MAX_BUFFER];
    char srcTeacher[MAX_BUFFER];
    char srcResult[MAX_BUFFER];
    char srcExam[MAX_BUFFER];
    char srcRoutine[MAX_BUFFER];
    char srcAttendance[MAX_BUFFER];
    char srcFee[MAX_BUFFER];
    char srcSystemConfig[MAX_BUFFER];

    snprintf(srcUser, sizeof(srcUser), "%susers_%s.bak", BACKUP_DIR, backupDate);
    snprintf(srcStudent, sizeof(srcStudent), "%sstudents_%s.bak", BACKUP_DIR, backupDate);
    snprintf(srcTeacher, sizeof(srcTeacher), "%steachers_%s.bak", BACKUP_DIR, backupDate);
    snprintf(srcResult, sizeof(srcResult), "%sresults_%s.bak", BACKUP_DIR, backupDate);
    snprintf(srcExam, sizeof(srcExam), "%sexams_%s.bak", BACKUP_DIR, backupDate);
    snprintf(srcRoutine, sizeof(srcRoutine), "%sroutines_%s.bak", BACKUP_DIR, backupDate);
    snprintf(srcAttendance, sizeof(srcAttendance), "%sattendances_%s.bak", BACKUP_DIR, backupDate);
    snprintf(srcFee, sizeof(srcFee), "%sfees_%s.bak", BACKUP_DIR, backupDate);
    snprintf(srcSystemConfig, sizeof(srcSystemConfig), "%ssystemConfig_%s.bak", BACKUP_DIR, backupDate);

    bool uR   = copyFile(srcUser, USER_DATA_FILE);
    bool sR   = copyFile(srcStudent, STUDENT_DATA_FILE);
    bool tR   = copyFile(srcTeacher, TEACHER_DATA_FILE);
    bool rR   = copyFile(srcResult, RESULT_DATA_FILE);
    bool eR   = copyFile(srcExam, EXAM_DATA_FILE);
    bool rtR  = copyFile(srcRoutine, ROUTINE_DATA_FILE);
    bool aR   = copyFile(srcAttendance, ATTENDANCE_DATA_FILE);
    bool fR   = copyFile(srcFee, FEE_DATA_FILE);
    
    copyFile(srcSystemConfig, SYSTEM_CONFIG_FILE);

    if(uR && sR && tR && rR && eR && rtR && aR && fR) {
        if(loadDatabase()) {
            return SYS_SUCCESS;
        }
    }
    return SYS_ERR_RESTORE_FAILED;
}
bool verifyDatabaseIntegrity(void) {
    if(userCount < 0 || studentCount < 0 || teacherCount < 0) {
        return false;
    }
    return true;
}

SystemResponse exportDatabaseToExcel(const char *destinationDir) {
    if(destinationDir == NULL || strlen(destinationDir) == 0) {
        return SYS_ERR_EXPORT_FAILED;
    }

    createDirectory(destinationDir);

    const char *systemFolderName = "Database_Export";
    char finalExportPath[MAX_BUFFER * 2];
    size_t pathLen = strlen(destinationDir);

    if(destinationDir[pathLen - 1] == '\\' || destinationDir[pathLen - 1] == '/') {
        snprintf(finalExportPath, sizeof(finalExportPath), "%s%s", destinationDir, systemFolderName);
    } else {
        snprintf(finalExportPath, sizeof(finalExportPath), "%s" WIN_PATH_SEP "%s", destinationDir, systemFolderName);
    }

    createDirectory(finalExportPath);

    char studentFilePath[MAX_BUFFER * 3];
    char teacherFilePath[MAX_BUFFER * 3];
    char userFilePath[MAX_BUFFER * 3];
    char feeFilePath[MAX_BUFFER * 3];
    char examFilePath[MAX_BUFFER * 3];
    char resultFilePath[MAX_BUFFER * 3];
    char routineFilePath[MAX_BUFFER * 3];
    char attendanceFilePath[MAX_BUFFER * 3];

    snprintf(studentFilePath, sizeof(studentFilePath), "%s" WIN_PATH_SEP "students_export.csv", finalExportPath);
    snprintf(teacherFilePath, sizeof(teacherFilePath), "%s" WIN_PATH_SEP "teachers_export.csv", finalExportPath);
    snprintf(userFilePath, sizeof(userFilePath), "%s" WIN_PATH_SEP "users_export.csv", finalExportPath);
    snprintf(feeFilePath, sizeof(feeFilePath), "%s" WIN_PATH_SEP "fees_export.csv", finalExportPath);
    snprintf(examFilePath, sizeof(examFilePath), "%s" WIN_PATH_SEP "exams_export.csv", finalExportPath);
    snprintf(resultFilePath, sizeof(resultFilePath), "%s" WIN_PATH_SEP "results_export.csv", finalExportPath);
    snprintf(routineFilePath, sizeof(routineFilePath), "%s" WIN_PATH_SEP "routines_export.csv", finalExportPath);
    snprintf(attendanceFilePath, sizeof(attendanceFilePath), "%s" WIN_PATH_SEP "attendances_export.csv", finalExportPath);

    /*---------- Export Student CSV ----------*/
    FILE *fStudent = fopen(studentFilePath, "w");
    if(fStudent) {
        fprintf(fStudent, "\xEF\xBB\xBF"); // UTF-8 BOM for excel
        fprintf(fStudent, "ID,Roll No,Name,Father's Name,Mother's Name,Class,Section,Academic Year,Phone,Email,DOB,Gender,Blood Group,Address,Admission Date,Status\n");

        for(int i = 0; i < studentCount; i++) {
            char formattedName[MAX_STR_LEN * 2], formattedFather[MAX_STR_LEN * 2], formattedMother[MAX_STR_LEN * 2];
            char formattedEmail[MAX_STR_LEN * 2], formattedPhone[MAX_PHONE_LEN * 2], formattedAddress[MAX_BUFFER * 2], formattedSection[20];

            formatExcelField(formattedName, studentDB[i].name, sizeof(formattedName));
            formatExcelField(formattedFather, studentDB[i].fatherName, sizeof(formattedFather));
            formatExcelField(formattedMother, studentDB[i].motherName, sizeof(formattedMother));
            formatExcelField(formattedEmail, studentDB[i].email, sizeof(formattedEmail));
            formatExcelField(formattedPhone, studentDB[i].phone, sizeof(formattedPhone));
            formatExcelField(formattedAddress, studentDB[i].address, sizeof(formattedAddress));
            formatExcelField(formattedSection, studentDB[i].section, sizeof(formattedSection));

            fprintf(fStudent, "%d,%d,%s,%s,%s,%d,%s,%d,%s,%s,%s,%s,%s,%s,%s,%s\n",
                    studentDB[i].id, studentDB[i].rollNumber, formattedName, formattedFather, formattedMother,
                    studentDB[i].currentClass, formattedSection, studentDB[i].academicYear, formattedPhone,
                    formattedEmail, studentDB[i].dateOfBirth, studentDB[i].gender, studentDB[i].bloodGroup,
                    formattedAddress, studentDB[i].admissionDate, studentDB[i].isActive ? "Active" : "Inactive");
        }
        fclose(fStudent);
    }

    /*---------- Export Teacher CSV ----------*/
    FILE *fTeacher = fopen(teacherFilePath, "w");
    if(fTeacher) {
        fprintf(fTeacher, "\xEF\xBB\xBF");
        fprintf(fTeacher, "ID,Name,Email,Phone,Qualification,Assigned Subjects Count,Salary,Experience (Yrs),Joining Date,Status\n");

        for(int i = 0; i < teacherCount; i++) {
            char formattedName[MAX_STR_LEN * 2], formattedEmail[MAX_STR_LEN * 2], formattedPhone[MAX_PHONE_LEN * 2], formattedQual[MAX_STR_LEN * 2];

            formatExcelField(formattedName, teacherDB[i].name, sizeof(formattedName));
            formatExcelField(formattedEmail, teacherDB[i].email, sizeof(formattedEmail));
            formatExcelField(formattedPhone, teacherDB[i].phone, sizeof(formattedPhone));
            formatExcelField(formattedQual, teacherDB[i].qualification, sizeof(formattedQual));

            fprintf(fTeacher, "%d,%s,%s,%s,%s,%d,%.2f,%d,%s,%s\n",
                    teacherDB[i].id, formattedName, formattedEmail, formattedPhone, formattedQual,
                    teacherDB[i].assignedSubjectCount, teacherDB[i].salary, teacherDB[i].experience,
                    teacherDB[i].joiningDate, teacherDB[i].isActive ? "Active" : "Inactive");
        }
        fclose(fTeacher);
    }

    /*---------- Export User Account CSV ----------*/
    FILE *fUser = fopen(userFilePath, "w");
    if(fUser) {
        fprintf(fUser, "\xEF\xBB\xBF");
        fprintf(fUser, "ID,Username,Role,Admin Type,Linked ID,Status\n");

        for(int i = 0; i < userCount; i++) {
            char formattedUsername[MAX_STR_LEN * 2];
            formatExcelField(formattedUsername, userDB[i].username, sizeof(formattedUsername));

            const char *roleStr = (userDB[i].role == ROLE_ADMIN) ? "Admin" : (userDB[i].role == ROLE_TEACHER) ? "Teacher" : "Student";

            fprintf(fUser, "%d,%s,%s,%d,%d,%s\n",
                    userDB[i].id, formattedUsername, roleStr, userDB[i].adminType,
                    userDB[i].linkedId, userDB[i].isActive ? "Active" : "Inactive");
        }
        fclose(fUser);
    }

    /*---------- Export Fee CSV ----------*/
    FILE *fFee = fopen(feeFilePath, "w");
    if(fFee) {
        fprintf(fFee, "\xEF\xBB\xBF");
        fprintf(fFee, "Fee ID,Student ID,Class,Fee Type,Total Fee,Paid Amount,Due Amount,Status,Last Payment Date,Active Status\n");

        for(int i = 0; i < feeCount; i++) {
            char formattedType[MAX_STR_LEN * 2];
            formatExcelField(formattedType, feeDB[i].feeType, sizeof(formattedType));

            const char *statusStr = (feeDB[i].status == STATUS_PAID) ? "Paid" : (feeDB[i].status == STATUS_PARTIAL) ? "Partial" : "Unpaid";

            fprintf(fFee, "%d,%d,%d,%s,%.2f,%.2f,%.2f,%s,%s,%s\n",
                    feeDB[i].id, feeDB[i].studentId, feeDB[i].classNumber, formattedType,
                    feeDB[i].feeTotal, feeDB[i].feePaid, feeDB[i].feeDue, statusStr,
                    feeDB[i].lastPaymentDate, feeDB[i].isActive ? "Active" : "Inactive");
        }
        fclose(fFee);
    }

    /*---------- Export Exam CSV ----------*/
    FILE *fExam = fopen(examFilePath, "w");
    if(fExam) {
        fprintf(fExam, "\xEF\xBB\xBF");
        fprintf(fExam, "Exam ID,Class,Subject,Exam Date,Total Marks\n");

        for(int i = 0; i < examCount; i++) {
            char formattedSubject[MAX_STR_LEN * 2];
            formatExcelField(formattedSubject, examDB[i].subject, sizeof(formattedSubject));

            fprintf(fExam, "%d,%d,%s,%s,%.2f\n",
                    examDB[i].examId, examDB[i].classNumber, formattedSubject,
                    examDB[i].examDate, examDB[i].totalMarks);
        }
        fclose(fExam);
    }

    /*---------- Export Result CSV ----------*/
    FILE *fResult = fopen(resultFilePath, "w");
    if(fResult) {
        fprintf(fResult, "\xEF\xBB\xBF");
        fprintf(fResult, "Result ID,Student ID,Class,Exam Term,Year,Total GPA,Subject Count,Status\n");

        for(int i = 0; i < resultCount; i++) {
            char formattedTerm[MAX_STR_LEN * 2];
            formatExcelField(formattedTerm, resultDB[i].examTerm, sizeof(formattedTerm));

            fprintf(fResult, "%d,%d,%d,%s,%d,%.2f,%d,%s\n",
                    resultDB[i].id, resultDB[i].studentId, resultDB[i].classNumber,
                    formattedTerm, resultDB[i].year, resultDB[i].totalGPA,
                    resultDB[i].subjectCount, resultDB[i].isActive ? "Active" : "Inactive");
        }
        fclose(fResult);
    }

    /*---------- Export Routine CSV ----------*/
    FILE *fRoutine = fopen(routineFilePath, "w");
    if(fRoutine) {
        fprintf(fRoutine, "\xEF\xBB\xBF");
        fprintf(fRoutine, "Routine ID,Class,Section,Day of Week,Period No,Time Slot,Subject,Teacher ID\n");

        for(int i = 0; i < routineCount; i++) {
            char formattedSection[20], formattedDay[20], formattedSlot[40], formattedSubject[MAX_STR_LEN * 2];
            formatExcelField(formattedSection, routineDB[i].section, sizeof(formattedSection));
            formatExcelField(formattedDay, routineDB[i].dayOfWeek, sizeof(formattedDay));
            formatExcelField(formattedSlot, routineDB[i].timeSlot, sizeof(formattedSlot));
            formatExcelField(formattedSubject, routineDB[i].subjectName, sizeof(formattedSubject));

            fprintf(fRoutine, "%d,%d,%s,%s,%d,%s,%s,%d\n",
                    routineDB[i].id, routineDB[i].classNumber, formattedSection,
                    formattedDay, routineDB[i].periodNumber, formattedSlot,
                    formattedSubject, routineDB[i].teacherId);
        }
        fclose(fRoutine);
    }

    /*---------- Export Attendance CSV ----------*/
    FILE *fAttendance = fopen(attendanceFilePath, "w");
    if(fAttendance) {
        fprintf(fAttendance, "\xEF\xBB\xBF");
        fprintf(fAttendance, "Attendance ID,Student ID,Class,Date,Is Present\n");

        for(int i = 0; i < attendanceCount; i++) {
            fprintf(fAttendance, "%d,%d,%d,%s,%s\n",
                    attendanceDB[i].attendanceId, attendanceDB[i].studentId,
                    attendanceDB[i].classNumber, attendanceDB[i].date,
                    attendanceDB[i].isPresent ? "Present" : "Absent");
        }
        fclose(fAttendance);
    }

    return SYS_SUCCESS;
}
