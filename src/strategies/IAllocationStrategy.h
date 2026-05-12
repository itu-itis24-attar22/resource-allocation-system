#ifndef IALLOCATIONSTRATEGY_H
#define IALLOCATIONSTRATEGY_H

#include <vector>
#include "../models/Allocation.h"
#include "../models/Request.h"
#include "../models/OneTimeRequest.h"
#include "../models/RecurringRequest.h"
#include "../models/ExamRequest.h"
#include "../models/CommitteeMeetingRequest.h"
#include "../models/Space.h"
#include "../rules/RuleEngineFacade.h"

class IAllocationStrategy {
public:
    virtual ~IAllocationStrategy() = default;

    virtual void processRequests(const std::vector<Request*>& requests,
                                 const std::vector<Space*>& spaces,
                                 std::vector<Allocation>& allocations,
                                 const RuleEngineFacade& ruleEngineFacade) const = 0;

    virtual bool processRequest(OneTimeRequest& request,
                                std::vector<Allocation>& allocations,
                                const RuleEngineFacade& ruleEngineFacade) const = 0;

    virtual bool processRequest(RecurringRequest& request,
                                std::vector<Allocation>& allocations,
                                const RuleEngineFacade& ruleEngineFacade) const = 0;

    virtual bool processRequest(ExamRequest& request,
                                std::vector<Allocation>& allocations,
                                const RuleEngineFacade& ruleEngineFacade) const = 0;

    virtual bool processRequest(CommitteeMeetingRequest& request,
                                std::vector<Allocation>& allocations,
                                const RuleEngineFacade& ruleEngineFacade) const = 0;
};

#endif
