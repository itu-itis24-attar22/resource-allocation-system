#ifndef ALLOCATIONSERVICE_H
#define ALLOCATIONSERVICE_H

#include <string>
#include <vector>
#include "../models/Allocation.h"
#include "../models/Request.h"
#include "../models/Space.h"
#include "../models/OneTimeRequest.h"
#include "../models/RecurringRequest.h"
#include "../models/ExamRequest.h"
#include "../models/CommitteeMeetingRequest.h"
#include "../rules/RuleEngineFacade.h"
#include "../strategies/IAllocationStrategy.h"

class AllocationService {
private:
    std::vector<Allocation> allocations;
    RuleEngineFacade ruleEngineFacade;
    const IAllocationStrategy* allocationStrategy;

public:
    AllocationService();
    explicit AllocationService(const std::string& strategyName);
    const std::vector<Allocation>& getAllocations() const;
    void addExistingAllocation(const Allocation& allocation);
    void processRequests(const std::vector<Request*>& requests);
    void processRequests(const std::vector<Request*>& requests,
                         const std::vector<Space*>& spaces);
    bool processRequest(OneTimeRequest& request);
    bool processRequest(RecurringRequest& request);
    bool processRequest(ExamRequest& request);
    bool processRequest(CommitteeMeetingRequest& request);
    void printAllocations() const;
};

#endif
