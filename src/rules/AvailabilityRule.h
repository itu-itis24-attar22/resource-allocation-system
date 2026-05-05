#ifndef AVAILABILITYRULE_H
#define AVAILABILITYRULE_H

#include <vector>
#include "../models/OneTimeRequest.h"
#include "../models/RecurringRequest.h"
#include "../models/ExamRequest.h"
#include "../models/Allocation.h"

class AvailabilityRule {
private:
    bool isSpaceAvailable(const Space* space, const TimeSlot& timeSlot, const std::vector<Allocation>& existingAllocations) const;

public:
    bool check(const OneTimeRequest& request, const std::vector<Allocation>& existingAllocations) const;
    bool check(const RecurringRequest& request, const std::vector<Allocation>& existingAllocations) const;
    bool check(const ExamRequest& request, const std::vector<Allocation>& existingAllocations) const;
};

#endif
