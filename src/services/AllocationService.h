#ifndef ALLOCATIONSERVICE_H
#define ALLOCATIONSERVICE_H

#include <vector>
#include "Allocation.h"
#include "AvailabilityRule.h"
#include "OneTimeRequest.h"

class AllocationService {
private:
    std::vector<Allocation> allocations;
    AvailabilityRule rule;
    int nextAllocationId;

public:
    AllocationService();

    void addExistingAllocation(const Allocation& allocation);
    bool processRequest(OneTimeRequest& request);
    void printAllocations() const;
};

#endif