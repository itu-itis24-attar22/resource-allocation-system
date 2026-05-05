#ifndef RULEENGINE_H
#define RULEENGINE_H

#include <vector>
#include "IRequestRule.h"
#include "AvailabilityRule.h"
#include "RuleEvaluationResult.h"
#include "../models/OneTimeRequest.h"
#include "../models/RecurringRequest.h"
#include "../models/ExamRequest.h"
#include "../models/Allocation.h"

class RuleEngine {
private:
    std::vector<const IRequestRule*> requestRules;
    AvailabilityRule availabilityRule;

public:
    RuleEngine(const std::vector<const IRequestRule*>& requestRules);

    RuleEvaluationResult evaluate(OneTimeRequest& request,
                                  const std::vector<Allocation>& allocations) const;

    RuleEvaluationResult evaluate(RecurringRequest& request,
                                  const std::vector<Allocation>& allocations) const;

    RuleEvaluationResult evaluate(ExamRequest& request,
                                  const std::vector<Allocation>& allocations) const;
};

#endif
