#ifndef IALLOCATIONSTRATEGY_H
#define IALLOCATIONSTRATEGY_H

#include <vector>
#include "../models/Allocation.h"
#include "../models/Request.h"
#include "../models/OneTimeRequest.h"
#include "../models/RecurringRequest.h"
#include "../rules/RuleEngineFacade.h"

class IAllocationStrategy {
public:
    virtual ~IAllocationStrategy() = default;

    virtual void processRequests(const std::vector<Request*>& requests,
                                 std::vector<Allocation>& allocations,
                                 const RuleEngineFacade& ruleEngineFacade) const = 0;

    virtual bool processRequest(OneTimeRequest& request,
                                std::vector<Allocation>& allocations,
                                const RuleEngineFacade& ruleEngineFacade) const = 0;

    virtual bool processRequest(RecurringRequest& request,
                                std::vector<Allocation>& allocations,
                                const RuleEngineFacade& ruleEngineFacade) const = 0;
};

#endif
