#ifndef GREEDYALLOCATIONSTRATEGY_H
#define GREEDYALLOCATIONSTRATEGY_H

#include "IAllocationStrategy.h"

class GreedyAllocationStrategy : public IAllocationStrategy {
public:
    bool processRequest(OneTimeRequest& request,
                        std::vector<Allocation>& allocations,
                        const RuleEngineFacade& ruleEngineFacade) const override;

    bool processRequest(RecurringRequest& request,
                        std::vector<Allocation>& allocations,
                        const RuleEngineFacade& ruleEngineFacade) const override;
};

#endif
