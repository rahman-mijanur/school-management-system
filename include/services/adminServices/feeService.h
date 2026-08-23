#ifndef FEE_SERVICE_H
#define FEE_SERVICE_H

#include "models/schoolFee.h"

int executeCollectFee(int studentId, float amountPaid);
int executeClassFeeStructureUpdate(int targetClass, float newClassFee);
int executeApplyWaiver(int studentId, float waiverAmount);
void calculateFeeMetrics(float *outTotalExpected, float *outTotalCollected, float *outTotalDue);

#endif
