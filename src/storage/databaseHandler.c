#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utils/config.h"
#include "models/user.h"
#include "models/teacher.h"
#include "models/student.h"
#include "models/academic.h"
#include "models/schoolFee.h"
#include "storage/fileHandler.h"
#include "storage/databaseHandler.h"

/* Global Storage Instances */
UserAccount userDB[MAX_USERS];
int userCount = 0;

Student studentDB[MAX_STUDENTS];
int studentCount = 0;

Teacher teacherDB[MAX_TEACHERS];
int teacherCount = 0;

Result resultDB[MAX_RESULTS];
int resultCount = 0;

Exam examDB[MAX_ROUTINES];
int examCount = 0;

Routine routineDB[MAX_ROUTINES];
int routineCount = 0;

Attendance attendanceDB[MAX_STUDENTS * 30];
int attendanceCount = 0;

SchoolFee feeDB[MAX_STUDENTS];
int feeCount = 0;

/* Auto-increment ID Generators Defaults */
int nextStudentId = 241001;
int nextTeacherId = 1001;
int nextResultId = 5001;
int nextExamId = 3001;
int nextRoutineId = 101;
int nextAttendanceId = 7001;
int nextFeeId = 9001;

static void createDefaultAdmin(void) {
    userCount = 0;
    teacherCount = 0;
    studentCount = 0;
    routineCount = 0;
    resultCount = 0;
    feeCount = 0;
    attendanceCount = 0;

    /* =========================================================================
       1. SEED ADMIN ACCOUNTS (3 Admins)
       ========================================================================= */
    // Admin 1: Super Admin
    memset(&userDB[userCount], 0, sizeof(UserAccount));
    userDB[userCount].id = userCount + 1;
    snprintf(userDB[userCount].username, sizeof(userDB[userCount].username), "mijan");
    snprintf(userDB[userCount].password, sizeof(userDB[userCount].password), "rootpass");
    userDB[userCount].role = ROLE_ADMIN;
    userDB[userCount].adminType = ADMIN_SUPER;
    userDB[userCount].linkedId = -1;
    userDB[userCount].isActive = true;
    userCount++;

    // Admin 2: Academic Admin
    memset(&userDB[userCount], 0, sizeof(UserAccount));
    userDB[userCount].id = userCount + 1;
    snprintf(userDB[userCount].username, sizeof(userDB[userCount].username), "rahim_admin");
    snprintf(userDB[userCount].password, sizeof(userDB[userCount].password), "admin123");
    userDB[userCount].role = ROLE_ADMIN;
    userDB[userCount].adminType = ADMIN_STANDARD;
    userDB[userCount].linkedId = -1;
    userDB[userCount].isActive = true;
    userCount++;

    // Admin 3: Accounts Admin
    memset(&userDB[userCount], 0, sizeof(UserAccount));
    userDB[userCount].id = userCount + 1;
    snprintf(userDB[userCount].username, sizeof(userDB[userCount].username), "karim_admin");
    snprintf(userDB[userCount].password, sizeof(userDB[userCount].password), "admin123");
    userDB[userCount].role = ROLE_ADMIN;
    userDB[userCount].adminType = ADMIN_STANDARD;
    userDB[userCount].linkedId = -1;
    userDB[userCount].isActive = true;
    userCount++;

    /* =========================================================================
       2. SEED TEACHERS & TEACHER USER ACCOUNTS (5 Teachers)
       ========================================================================= */
    // Teacher Data Arrays for concise loop seeding
    struct {
        int id;
        const char *name;
        const char *email;
        const char *phone;
        const char *address;
        const char *qual;
        const char *username;
        float salary;
        int exp;
        const char *joining;
        int subCount;
        struct { int classNum; const char *sec; const char *sub; } subs[2];
    } teacherSeed[10] = {
        {1001, "Md. Asib", "info@asib.com", "01700000044", "Dhanmondi, Dhaka", "M.Sc. in CSE", "asib", 45000.00f, 8, "2018-01-15", 2, {{10, "A", "Math"}, {9, "B", "Science"}}},
        {1002, "Rafiqul Islam", "rafiqul@school.edu", "01700000045", "Gulshan, Dhaka", "M.A. in English", "rafiqul", 42000.00f, 6, "2019-03-20", 2, {{10, "A", "English"}, {8, "A", "English"}}},
        {1003, "Tanvir Ahmed", "tanvir@school.edu", "01700000046", "Uttara, Dhaka", "M.Sc. in Physics", "tanvir", 48000.00f, 10, "2016-08-10", 2, {{10, "B", "Physics"}, {9, "A", "Physics"}}},
        {1004, "Mahmudur Rahman", "mahmudur@school.edu", "01700000047", "Banani, Dhaka", "M.Sc. in Chemistry", "mahmudur", 40000.00f, 5, "2020-02-01", 2, {{10, "A", "Chemistry"}, {9, "B", "Chemistry"}}},
        {1005, "Kamrul Hassan", "kamrul@school.edu", "01700000048", "Mirpur, Dhaka", "M.A. in History", "kamrul", 38000.00f, 4, "2021-05-12", 2, {{8, "A", "Social Science"}, {8, "B", "History"}}},
        {1006, "Tariqul Islam", "tariqul@school.edu", "01700000049", "Mohakhali, Dhaka", "M.Sc. in ICT", "tariqul", 46000.00f, 7, "2018-09-01", 2, {{10, "A", "ICT"}, {9, "A", "ICT"}}},
        {1007, "Shafiqul Chowdhury", "shafiqul@school.edu", "01700000050", "Badda, Dhaka", "M.Sc. in Biology", "shafiqul", 43000.00f, 6, "2019-11-15", 2, {{10, "B", "Biology"}, {9, "B", "Biology"}}},
        {1008, "Ashraful Alam", "ashraful@school.edu", "01700000051", "Khilgaon, Dhaka", "M.A. in Bengali", "ashraful", 39000.00f, 5, "2020-07-10", 2, {{7, "A", "Bengali"}, {8, "B", "Bengali"}}},
        {1009, "Zahid Hossain", "zahid@school.edu", "01700000052", "Bashundhara, Dhaka", "M.Sc. in Accounting", "zahid", 44000.00f, 7, "2018-04-05", 2, {{10, "C", "Accounting"}, {9, "C", "Commerce"}}},
        {1010, "Nasir Uddin", "nasir@school.edu", "01700000053", "Malibagh, Dhaka", "M.A. in Islamic Studies", "nasir", 37000.00f, 3, "2022-01-20", 2, {{6, "A", "Religion"}, {7, "B", "Religion"}}}
    };

    for(int i = 0; i < 10; i++) {
        memset(&teacherDB[i], 0, sizeof(Teacher));
        teacherDB[i].id = teacherSeed[i].id;
        snprintf(teacherDB[i].name, sizeof(teacherDB[i].name), "%s", teacherSeed[i].name);
        snprintf(teacherDB[i].email, sizeof(teacherDB[i].email), "%s", teacherSeed[i].email);
        snprintf(teacherDB[i].phone, sizeof(teacherDB[i].phone), "%s", teacherSeed[i].phone);
        snprintf(teacherDB[i].address, sizeof(teacherDB[i].address), "%s", teacherSeed[i].address);
        snprintf(teacherDB[i].qualification, sizeof(teacherDB[i].qualification), "%s", teacherSeed[i].qual);
        teacherDB[i].salary = teacherSeed[i].salary;
        teacherDB[i].experience = teacherSeed[i].exp;
        snprintf(teacherDB[i].joiningDate, sizeof(teacherDB[i].joiningDate), "%s", teacherSeed[i].joining);
        teacherDB[i].isActive = true;

        teacherDB[i].assignedSubjectCount = teacherSeed[i].subCount;
        for(int j = 0; j < teacherSeed[i].subCount; j++) {
            teacherDB[i].assignedSubjects[j].classNumber = teacherSeed[i].subs[j].classNum;
            snprintf(teacherDB[i].assignedSubjects[j].section, sizeof(teacherDB[i].assignedSubjects[j].section), "%s", teacherSeed[i].subs[j].sec);
            snprintf(teacherDB[i].assignedSubjects[j].subjectName, sizeof(teacherDB[i].assignedSubjects[j].subjectName), "%s", teacherSeed[i].subs[j].sub);
        }
        teacherCount++;

        // Linked Teacher User Account
        memset(&userDB[userCount], 0, sizeof(UserAccount));
        userDB[userCount].id = userCount + 1;
        snprintf(userDB[userCount].username, sizeof(userDB[userCount].username), "%s", teacherSeed[i].username);
        snprintf(userDB[userCount].password, sizeof(userDB[userCount].password), "teacher123");
        userDB[userCount].role = ROLE_TEACHER;
        userDB[userCount].adminType = ADMIN_NONE;
        userDB[userCount].linkedId = teacherSeed[i].id;
        userDB[userCount].isActive = true;
        userCount++;
    }

    /* =========================================================================
       3. SEED STUDENTS & STUDENT USER ACCOUNTS (10 Students)
       ========================================================================= */
    struct {
        int id;
        int roll;
        const char *name;
        const char *father;
        const char *mother;
        const char *email;
        const char *phone;
        const char *address;
        const char *dob;
        const char *gender;
        const char *blood;
        int classNum;
        const char *sec;
        const char *username;
    } studentSeed[20] = {
        {241001, 1, "Md. Abid Mia", "Anisur Rahman", "Rehana Begum", "info@abid.com", "01900000066", "Mirpur, Dhaka", "2010-05-14", "Male", "B+", 10, "A", "abid"},
        {241002, 2, "Sabbir Hossain", "Belal Hossain", "Sharmin Sultana", "sabbir@mail.com", "01900000067", "Dhanmondi, Dhaka", "2010-08-22", "Male", "A+", 10, "A", "sabbir"},
        {241003, 3, "Tanvir Ahmed", "Khorshed Alam", "Fatema Khatun", "tanvir@mail.com", "01900000068", "Uttara, Dhaka", "2010-03-11", "Male", "O+", 10, "A", "tanvir_s"},
        {241004, 4, "Taskin Ahmed", "Mustafizur Rahman", "Nasima Akter", "taskin@mail.com", "01900000069", "Mohakhali, Dhaka", "2010-11-05", "Male", "AB+", 10, "B", "taskin"},
        {241005, 5, "Rakibul Hasan", "Zakir Hossain", "Roxana Begum", "rakib@mail.com", "01900000070", "Badda, Dhaka", "2010-01-30", "Male", "B-", 10, "B", "rakib"},
        {241006, 1, "Mehedi Hasan", "Jahangir Alam", "Kohinur Begum", "mehedi@mail.com", "01900000071", "Mirpur, Dhaka", "2011-07-19", "Male", "O-", 9, "A", "mehedi"},
        {241007, 2, "Shahriar Alam", "Mahbubur Rahman", "Laila Arjumand", "shahriar@mail.com", "01900000072", "Khilgaon, Dhaka", "2011-09-12", "Male", "A+", 9, "A", "shahriar"},
        {241008, 3, "Arifur Rahman", "Habibur Rahman", "Salma Khatun", "arif@mail.com", "01900000073", "Shyamoli, Dhaka", "2011-04-02", "Male", "B+", 9, "B", "arif"},
        {241009, 1, "Tamim Iqbal", "Anwar Hossain", "Bilkis Banu", "tamim@mail.com", "01900000074", "Rampura, Dhaka", "2012-12-15", "Male", "A-", 8, "A", "tamim"},
        {241010, 2, "Jahid Mahmud", "Shahadat Hossain", "Monowara Begum", "jahid@mail.com", "01900000075", "Lalmatia, Dhaka", "2012-06-25", "Male", "O+", 8, "A", "jahid"},
        {241011, 3, "Naimur Rahman", "Fazlur Rahman", "Parvin Akter", "naimur@mail.com", "01900000076", "Gulshan, Dhaka", "2010-02-18", "Male", "AB-", 10, "B", "naimur"},
        {241012, 4, "Mahfuzur Rahman", "Aminul Islam", "Rina Begum", "mahfuz@mail.com", "01900000077", "Banani, Dhaka", "2011-05-09", "Male", "B+", 9, "B", "mahfuz"},
        {241013, 3, "Fahim Islam", "Nazmul Huda", "Sultana Razia", "fahim@mail.com", "01900000078", "Malibagh, Dhaka", "2012-08-30", "Male", "A+", 8, "B", "fahim"},
        {241014, 1, "Naim Islam", "Rafiqul Islam", "Momotaz Begum", "naim@mail.com", "01900000079", "Bashundhara, Dhaka", "2013-04-12", "Male", "O+", 7, "A", "naim"},
        {241015, 2, "Shakib Al Hasan", "Masud Hasan", "Shirina Akter", "shakib@mail.com", "01900000080", "Tejgaon, Dhaka", "2013-10-08", "Male", "B+", 7, "A", "shakib"},
        {241016, 3, "Hasibur Rahman", "Mizanur Rahman", "Rashida Begum", "hasib@mail.com", "01900000081", "Jatrabari, Dhaka", "2013-01-25", "Male", "AB+", 7, "B", "hasib"},
        {241017, 1, "Siam Hossain", "Kabir Hossain", "Asma Begum", "siam@mail.com", "01900000082", "Kamalapur, Dhaka", "2014-06-14", "Male", "A-", 6, "A", "siam"},
        {241018, 2, "Rayhan Ahmed", "Kamrul Islam", "Nargis Akter", "rayhan@mail.com", "01900000083", "Mogbazar, Dhaka", "2014-09-03", "Male", "O-", 6, "A", "rayhan"},
        {241019, 3, "Abrar Zahin", "Zahirul Islam", "Farida Yasmin", "abrar@mail.com", "01900000084", "Lalbagh, Dhaka", "2014-11-20", "Male", "B+", 6, "B", "abrar"},
        {241020, 4, "Tawsif Islam", "Shajahan Ali", "Hazera Begum", "tawsif@mail.com", "01900000085", "Savar, Dhaka", "2014-03-05", "Male", "AB-", 6, "B", "tawsif"}
    };

    for(int i = 0; i < 20; i++) {
        memset(&studentDB[i], 0, sizeof(Student));
        studentDB[i].id = studentSeed[i].id;
        studentDB[i].rollNumber = studentSeed[i].roll;
        snprintf(studentDB[i].name, sizeof(studentDB[i].name), "%s", studentSeed[i].name);
        snprintf(studentDB[i].fatherName, sizeof(studentDB[i].fatherName), "%s", studentSeed[i].father);
        snprintf(studentDB[i].motherName, sizeof(studentDB[i].motherName), "%s", studentSeed[i].mother);
        snprintf(studentDB[i].email, sizeof(studentDB[i].email), "%s", studentSeed[i].email);
        snprintf(studentDB[i].phone, sizeof(studentDB[i].phone), "%s", studentSeed[i].phone);
        snprintf(studentDB[i].address, sizeof(studentDB[i].address), "%s", studentSeed[i].address);
        snprintf(studentDB[i].dateOfBirth, sizeof(studentDB[i].dateOfBirth), "%s", studentSeed[i].dob);
        snprintf(studentDB[i].gender, sizeof(studentDB[i].gender), "%s", studentSeed[i].gender);
        snprintf(studentDB[i].bloodGroup, sizeof(studentDB[i].bloodGroup), "%s", studentSeed[i].blood);
        studentDB[i].currentClass = studentSeed[i].classNum;
        snprintf(studentDB[i].section, sizeof(studentDB[i].section), "%s", studentSeed[i].sec);
        studentDB[i].academicYear = 2026;
        snprintf(studentDB[i].admissionDate, sizeof(studentDB[i].admissionDate), "2024-01-10");
        studentDB[i].isActive = true;
        studentCount++;

        // Linked Student User Account
        memset(&userDB[userCount], 0, sizeof(UserAccount));
        userDB[userCount].id = userCount + 1;
        snprintf(userDB[userCount].username, sizeof(userDB[userCount].username), "%s", studentSeed[i].username);
        snprintf(userDB[userCount].password, sizeof(userDB[userCount].password), "student123");
        userDB[userCount].role = ROLE_STUDENT;
        userDB[userCount].adminType = ADMIN_NONE;
        userDB[userCount].linkedId = studentSeed[i].id;
        userDB[userCount].isActive = true;
        userCount++;
    }

    /* =========================================================================
       4. SEED ROUTINES
       ========================================================================= */
    struct {
        int id; int classNum; const char *sec; const char *day; int period; const char *time; const char *sub; int teacherId;
    } routineSeed[] = {
        {101, 10, "A", "Sunday",    1, "09:00AM - 09:45AM", "Math",           1001},
        {102, 10, "A", "Sunday",    2, "09:45AM - 10:30AM", "English",        1002},
        {103, 10, "A", "Monday",    1, "09:00AM - 09:45AM", "Chemistry",      1004},
        {104, 10, "B", "Sunday",    1, "09:00AM - 09:45AM", "Physics",        1003},
        {105, 9,  "A", "Monday",    2, "09:45AM - 10:30AM", "Physics",        1003},
        {106, 9,  "B", "Tuesday",   1, "09:00AM - 09:45AM", "Science",        1001},
        {107, 8,  "A", "Sunday",    1, "09:00AM - 09:45AM", "English",        1002},
        {108, 8,  "A", "Sunday",    2, "09:45AM - 10:30AM", "Social Science", 1005},
        {109, 10, "A", "Sunday",    3, "10:45AM - 11:30AM", "ICT",            1006},
        {110, 10, "B", "Sunday",    2, "09:45AM - 10:30AM", "Biology",        1007},
        {111, 9,  "A", "Monday",    1, "09:00AM - 09:45AM", "ICT",            1006},
        {112, 9,  "B", "Tuesday",   2, "09:45AM - 10:30AM", "Biology",        1007},
        {113, 8,  "A", "Monday",    1, "09:00AM - 09:45AM", "Bengali",        1008},
        {114, 8,  "B", "Monday",    2, "09:45AM - 10:30AM", "History",        1005},
        {115, 10, "C", "Sunday",    1, "09:00AM - 09:45AM", "Accounting",     1009},
        {116, 9,  "C", "Sunday",    2, "09:45AM - 10:30AM", "Commerce",       1009},
        {117, 7,  "A", "Wednesday", 1, "09:00AM - 09:45AM", "Bengali",        1008},
        {118, 7,  "B", "Wednesday", 2, "09:45AM - 10:30AM", "Religion",       1010},
        {119, 6,  "A", "Thursday",  1, "09:00AM - 09:45AM", "Religion",       1010},
        {120, 10, "A", "Tuesday",   1, "09:00AM - 09:45AM", "Chemistry",      1004}
    };
    int totalRoutines = sizeof(routineSeed) / sizeof(routineSeed[0]);

    for(int i = 0; i < totalRoutines; i++) {
        memset(&routineDB[i], 0, sizeof(Routine));
        routineDB[i].id = routineSeed[i].id;
        routineDB[i].classNumber = routineSeed[i].classNum;
        snprintf(routineDB[i].section, sizeof(routineDB[i].section), "%s", routineSeed[i].sec);
        snprintf(routineDB[i].dayOfWeek, sizeof(routineDB[i].dayOfWeek), "%s", routineSeed[i].day);
        routineDB[i].periodNumber = routineSeed[i].period;
        snprintf(routineDB[i].timeSlot, sizeof(routineDB[i].timeSlot), "%s", routineSeed[i].time);
        snprintf(routineDB[i].subjectName, sizeof(routineDB[i].subjectName), "%s", routineSeed[i].sub);
        routineDB[i].teacherId = routineSeed[i].teacherId;
        routineCount++;
    }

    /* =========================================================================
       5. SEED RESULTS FOR ALL 10 STUDENTS
       ========================================================================= */
    for(int i = 0; i < studentCount; i++) {
        memset(&resultDB[i], 0, sizeof(Result));
        resultDB[i].id = 5001 + i;
        resultDB[i].studentId = studentDB[i].id;
        resultDB[i].classNumber = studentDB[i].currentClass;
        snprintf(resultDB[i].examTerm, sizeof(resultDB[i].examTerm), "first-term");
        resultDB[i].year = 2026;
        resultDB[i].subjectCount = 2;

        // Subject 1
        snprintf(resultDB[i].subjects[0].subjectName, sizeof(resultDB[i].subjects[0].subjectName), "Math");
        resultDB[i].subjects[0].marksObtained = 80.0f + (i % 5) * 3.0f;
        resultDB[i].subjects[0].maxMarks = 100.0f;
        resultDB[i].subjects[0].gradePoint = 5.00f;
        snprintf(resultDB[i].subjects[0].letterGrade, sizeof(resultDB[i].subjects[0].letterGrade), "A+");

        // Subject 2
        snprintf(resultDB[i].subjects[1].subjectName, sizeof(resultDB[i].subjects[1].subjectName), "English");
        resultDB[i].subjects[1].marksObtained = 70.0f + (i % 4) * 4.0f;
        resultDB[i].subjects[1].maxMarks = 100.0f;
        resultDB[i].subjects[1].gradePoint = 4.00f;
        snprintf(resultDB[i].subjects[1].letterGrade, sizeof(resultDB[i].subjects[1].letterGrade), "A");

        resultDB[i].totalGPA = 4.50f - (i % 3) * 0.25f;
        resultDB[i].isActive = true;
        resultCount++;
    }

    /* =========================================================================
       6. SEED SCHOOL FEES FOR ALL 10 STUDENTS
       ========================================================================= */
    for(int i = 0; i < studentCount; i++) {
        memset(&feeDB[i], 0, sizeof(SchoolFee));
        feeDB[i].id = 9001 + i;
        feeDB[i].studentId = studentDB[i].id;
        feeDB[i].classNumber = studentDB[i].currentClass;
        snprintf(feeDB[i].feeType, sizeof(feeDB[i].feeType), "Tuition Fee");
        feeDB[i].feeTotal = 24000.00f;
        
        if(i % 3 == 0) {
            feeDB[i].feePaid = 24000.00f;
            feeDB[i].feeDue = 0.00f;
            feeDB[i].status = STATUS_PAID;
        } else if(i % 3 == 1) {
            feeDB[i].feePaid = 14000.00f;
            feeDB[i].feeDue = 10000.00f;
            feeDB[i].status = STATUS_PARTIAL;
        } else {
            feeDB[i].feePaid = 0.00f;
            feeDB[i].feeDue = 24000.00f;
            feeDB[i].status = STATUS_UNPAID;
        }

        snprintf(feeDB[i].lastPaymentDate, sizeof(feeDB[i].lastPaymentDate), "2026-02-10");
        feeDB[i].isActive = true;
        feeCount++;
    }

    saveDatabase();
}

bool loadDatabase(void) {
    userCount = readFromFile(USER_DATA_FILE, userDB, sizeof(UserAccount), MAX_USERS);
    studentCount = readFromFile(STUDENT_DATA_FILE, studentDB, sizeof(Student), MAX_STUDENTS);
    teacherCount = readFromFile(TEACHER_DATA_FILE, teacherDB, sizeof(Teacher), MAX_TEACHERS);
    resultCount = readFromFile(RESULT_DATA_FILE, resultDB, sizeof(Result), MAX_RESULTS);
    examCount = readFromFile(EXAM_DATA_FILE, examDB, sizeof(Exam), MAX_ROUTINES);
    routineCount = readFromFile(ROUTINE_DATA_FILE, routineDB, sizeof(Routine), MAX_ROUTINES);
    attendanceCount = readFromFile(ATTENDANCE_DATA_FILE, attendanceDB, sizeof(Attendance), MAX_STUDENTS * 30);
    feeCount = readFromFile(FEE_DATA_FILE, feeDB, sizeof(SchoolFee), MAX_STUDENTS);

    /* Integrity Check & Reset */
    if(userCount < 0 || userCount > MAX_USERS) userCount = 0;
    if(studentCount < 0 || studentCount > MAX_STUDENTS) studentCount = 0;
    if(teacherCount < 0 || teacherCount > MAX_TEACHERS) teacherCount = 0;
    if(resultCount < 0 || resultCount > MAX_RESULTS) resultCount = 0;
    if(examCount < 0 || examCount > MAX_ROUTINES) examCount = 0;
    if(routineCount < 0 || routineCount > MAX_ROUTINES) routineCount = 0;
    if(attendanceCount < 0) attendanceCount = 0;
    if(feeCount < 0 || feeCount > MAX_STUDENTS) feeCount = 0;

    if(userCount == 0) createDefaultAdmin();

    /* Recalculate Auto-Increment Primary Keys */
    for(int i = 0; i < studentCount; i++) {
        if(studentDB[i].id >= nextStudentId) nextStudentId = studentDB[i].id + 1;
    }

    for(int i = 0; i < teacherCount; i++) {
        if(teacherDB[i].id >= nextTeacherId) nextTeacherId = teacherDB[i].id + 1;
    }

    for(int i = 0; i < resultCount; i++) {
        if(resultDB[i].id >= nextResultId) nextResultId = resultDB[i].id + 1;
    }

    for(int i = 0; i < examCount; i++) {
        if(examDB[i].examId >= nextExamId) nextExamId = examDB[i].examId + 1;
    }

    for(int i = 0; i < routineCount; i++) {
        if(routineDB[i].id >= nextRoutineId) nextRoutineId = routineDB[i].id + 1;
    }

    for(int i = 0; i < attendanceCount; i++) {
        if(attendanceDB[i].attendanceId >= nextAttendanceId) nextAttendanceId = attendanceDB[i].attendanceId + 1;
    }

    for(int i = 0; i < feeCount; i++) {
        if(feeDB[i].id >= nextFeeId) nextFeeId = feeDB[i].id + 1;
    }

    return true;
}

bool saveDatabase(void) {
    bool uStatus = writeToFileAtomic(USER_DATA_FILE, userDB, sizeof(UserAccount), userCount);
    bool sStatus = writeToFileAtomic(STUDENT_DATA_FILE, studentDB, sizeof(Student), studentCount);
    bool tStatus = writeToFileAtomic(TEACHER_DATA_FILE, teacherDB, sizeof(Teacher), teacherCount);
    bool rStatus = writeToFileAtomic(RESULT_DATA_FILE, resultDB, sizeof(Result), resultCount);
    bool eStatus = writeToFileAtomic(EXAM_DATA_FILE, examDB, sizeof(Exam), examCount);
    bool rtnStatus = writeToFileAtomic(ROUTINE_DATA_FILE, routineDB, sizeof(Routine), routineCount);
    bool aStatus = writeToFileAtomic(ATTENDANCE_DATA_FILE, attendanceDB, sizeof(Attendance), attendanceCount);
    bool fStatus = writeToFileAtomic(FEE_DATA_FILE, feeDB, sizeof(SchoolFee), feeCount);

    return (uStatus && sStatus && tStatus && rStatus && eStatus && rtnStatus && aStatus && fStatus);
}
