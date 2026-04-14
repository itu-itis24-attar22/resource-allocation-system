#ifndef ALLOCATIONSERVICE_H
#define ALLOCATIONSERVICE_H

#include <vector>
#include "../models/Allocation.h"
#include "../models/OneTimeRequest.h"
#include "../models/RecurringRequest.h"
#include "../rules/RuleEngineFacade.h"

class AllocationService {
private:
    std::vector<Allocation> allocations;
    RuleEngineFacade ruleEngineFacade;

public:
    const std::vector<Allocation>& getAllocations() const;
    void addExistingAllocation(const Allocation& allocation);
    bool processRequest(OneTimeRequest& request);
    bool processRequest(RecurringRequest& request);
    void printAllocations() const;
};

#endif