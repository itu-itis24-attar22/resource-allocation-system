#ifndef MULTIROOMEXAMBESTFITSTRATEGY_H
#define MULTIROOMEXAMBESTFITSTRATEGY_H

#include "GreedyAllocationStrategy.h"
#include "IAllocationStrategy.h"

class MultiRoomExamBestFitStrategy : public IAllocationStrategy {
private:
    GreedyAllocationStrategy greedyStrategy;

    bool processSplittableExamRequest(ExamRequest& request,
                                      const std::vector<Space*>& spaces,
                                      std::vector<Allocation>& allocations) const;

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

    bool processRequest(CommitteeMeetingRequest& request,
                        std::vector<Allocation>& allocations,
                        const RuleEngineFacade& ruleEngineFacade) const override;
};

#endif
