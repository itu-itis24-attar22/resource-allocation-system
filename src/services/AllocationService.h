#ifndef ALLOCATIONSERVICE_H
#define ALLOCATIONSERVICE_H

#include <vector>
#include "../models/Allocation.h"
#include "../models/OneTimeRequest.h"
#include "../models/RecurringRequest.h"
#include "../rules/AvailabilityRule.h"
#include "../rules/CapacityRule.h"
#include "../rules/FeatureRule.h"
#include "../rules/StatusRule.h"
#include "../rules/LocationRule.h"

class AllocationService {
private:
    std::vector<Allocation> allocations;
    AvailabilityRule availabilityRule;
    CapacityRule capacityRule;
    FeatureRule featureRule;
    StatusRule statusRule;
    LocationRule locationRule;

public:
    void addExistingAllocation(const Allocation& allocation);
    bool processRequest(OneTimeRequest& request);
    bool processRequest(RecurringRequest& request);
    void printAllocations() const;
};

#endif