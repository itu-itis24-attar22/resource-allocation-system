#ifndef ALLOCATIONSERVICE_H
#define ALLOCATIONSERVICE_H

#include <vector>
#include "../models/Allocation.h"
#include "../rules/AvailabilityRule.h"
#include "../rules/CapacityRule.h"
#include "../models/OneTimeRequest.h"

class AllocationService {
private:
    std::vector<Allocation> allocations;
    AvailabilityRule availabilityRule;
    CapacityRule capacityRule;
    int nextAllocationId;

public:
    AllocationService();

    void addExistingAllocation(const Allocation& allocation);
    bool processRequest(OneTimeRequest& request);
    void printAllocations() const;
};

#endif