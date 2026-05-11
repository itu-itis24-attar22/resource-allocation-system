#ifndef SHAREDROOMEXAMBESTFITSTRATEGY_H
#define SHAREDROOMEXAMBESTFITSTRATEGY_H

#include <unordered_map>
#include "GreedyAllocationStrategy.h"
#include "IAllocationStrategy.h"

class SharedRoomExamBestFitStrategy : public IAllocationStrategy {
private:
    GreedyAllocationStrategy greedyStrategy;

    bool processSplittableExamRequest(
        ExamRequest& request,
        const std::vector<Space*>& spaces,
        std::vector<Allocation>& allocations,
        const std::unordered_map<int, const Request*>& requestById
    ) const;

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
