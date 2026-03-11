#ifndef AVAILABILITYRULE_H
#define AVAILABILITYRULE_H

#include <vector>
#include "../models/OneTimeRequest.h"
#include "../models/Allocation.h"

class AvailabilityRule {
public:
    bool check(const OneTimeRequest& request, const std::vector<Allocation>& existingAllocations) const;
};

#endif