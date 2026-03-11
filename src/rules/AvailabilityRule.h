#ifndef AVAILABILITYRULE_H
#define AVAILABILITYRULE_H

#include <vector>
#include "OneTimeRequest.h"
#include "Allocation.h"

class AvailabilityRule {
public:
    bool check(const OneTimeRequest& request, const std::vector<Allocation>& existingAllocations) const;
};

#endif