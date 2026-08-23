#ifndef SCHOOL_FEE_H
#define SCHOOL_FEE_H

#include <stdbool.h>
#include "utils/config.h"

typedef enum {
    STATUS_UNPAID = 0,
    STATUS_PARTIAL = 1,
    STATUS_PAID = 2
} FeeStatus;

typedef struct {
    int id;
    int studentId;
    int classNumber;
    char feeType[MAX_STR_LEN];
    
    float feeTotal;
    float feePaid;
    float feeDue;
    
    FeeStatus status;
    char lastPaymentDate[MAX_DATE_LEN];
    bool isActive;
} SchoolFee;

#endif
