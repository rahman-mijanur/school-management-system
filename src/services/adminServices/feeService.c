#include <stdio.h>
#include <string.h>

#include "utils/config.h"
#include "models/schoolFee.h"
#include "storage/databaseHandler.h"
#include "services/adminServices/feeService.h"

static int findFeeIndexByStudentId(int studentId) {
    for(int i = 0; i < feeCount; i++) {
        if(feeDB[i].studentId == studentId && feeDB[i].isActive) {
            return i;
        }
    }
    return -1;
}

int executeCollectFee(int studentId, float amountPaid) {
    int feeIdx = findFeeIndexByStudentId(studentId);
    if(feeIdx == -1) return -1; // Record not found

    feeDB[feeIdx].feePaid += amountPaid;
    feeDB[feeIdx].feeDue = feeDB[feeIdx].feeTotal - feeDB[feeIdx].feePaid;

    if(feeDB[feeIdx].feeDue <= 0.0f) {
        feeDB[feeIdx].feeDue = 0.0f;
        feeDB[feeIdx].status = STATUS_PAID;
    } else if(feeDB[feeIdx].feePaid > 0.0f) {
        feeDB[feeIdx].status = STATUS_PARTIAL;
    } else {
        feeDB[feeIdx].status = STATUS_UNPAID;
    }

    if(saveDatabase()) return 1;
    return 0;
}

int executeClassFeeStructureUpdate(int targetClass, float newClassFee) {
    int updatedCount = 0;

    for(int i = 0; i < feeCount; i++) {
        if(feeDB[i].isActive && feeDB[i].classNumber == targetClass) {
            feeDB[i].feeTotal = newClassFee;
            feeDB[i].feeDue = feeDB[i].feeTotal - feeDB[i].feePaid;

            if(feeDB[i].feeDue <= 0.0f) {
                feeDB[i].feeDue = 0.0f;
                feeDB[i].status = STATUS_PAID;
            } else if(feeDB[i].feePaid > 0.0f) {
                feeDB[i].status = STATUS_PARTIAL;
            } else {
                feeDB[i].status = STATUS_UNPAID;
            }
            updatedCount++;
        }
    }

    if(updatedCount > 0) {
        saveDatabase();
    }
    return updatedCount;
}

int executeApplyWaiver(int studentId, float waiverAmount) {
    int feeIdx = findFeeIndexByStudentId(studentId);
    if(feeIdx == -1) return -1;
    if(waiverAmount < 0.0f || waiverAmount > feeDB[feeIdx].feeTotal) return -2;

    feeDB[feeIdx].feeTotal -= waiverAmount;
    feeDB[feeIdx].feeDue = feeDB[feeIdx].feeTotal - feeDB[feeIdx].feePaid;

    if(feeDB[feeIdx].feeDue <= 0.0f) {
        feeDB[feeIdx].feeDue = 0.0f;
        feeDB[feeIdx].status = STATUS_PAID;
    } else if(feeDB[feeIdx].feePaid > 0.0f) {
        feeDB[feeIdx].status = STATUS_PARTIAL;
    } else {
        feeDB[feeIdx].status = STATUS_UNPAID;
    }

    if(saveDatabase()) return 1;
    return 0;
}

void calculateFeeMetrics(float *outTotalExpected, float *outTotalCollected, float *outTotalDue) {
    *outTotalExpected = 0.0f;
    *outTotalCollected = 0.0f;
    *outTotalDue = 0.0f;

    for(int i = 0; i < feeCount; i++) {
        if(!feeDB[i].isActive) continue;
        *outTotalExpected += feeDB[i].feeTotal;
        *outTotalCollected += feeDB[i].feePaid;
        *outTotalDue += feeDB[i].feeDue;
    }
}
