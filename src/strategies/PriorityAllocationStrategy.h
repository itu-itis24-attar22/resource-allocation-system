#ifndef PRIORITYALLOCATIONSTRATEGY_H
#define PRIORITYALLOCATIONSTRATEGY_H

#include "IAllocationStrategy.h"

class PriorityAllocationStrategy : public IAllocationStrategy {
public:
    void processRequests(const std::vector<Request*>& requests,
                         const std::vector<Space*>& spaces,
                         std::vector<Allocation>& allocations,
                         const RuleEngineFacade& ruleEngineFacade) const override;

    bool processRequest(OneTimeRequest& request,
                        std::vector<Allocation>& allocations,
                        const RuleEngineFacade& ruleEngineFacade) const override;

    bool processRequest(RecurringRequest& request,
                        std::vector<Allocation>& allocations,
                        const RuleEngineFacade& ruleEngineFacade) const override;

    bool processRequest(ExamRequest& request,
                        std::vector<Allocation>& allocations,
                        const RuleEngineFacade& ruleEngineFacade) const override;
};

#endif
