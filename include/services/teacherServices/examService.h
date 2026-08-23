#ifndef EXAM_SERVICE_H
#define EXAM_SERVICE_H

#include <stdbool.h>
#include "models/academic.h"

int getAllExams(Exam *examList, int maxExams);
int createExam(int classNumber, const char *subject, const char *examDate, float totalMarks);
bool assignStudentMarks(int examId, int studentId, float marks);

#endif
