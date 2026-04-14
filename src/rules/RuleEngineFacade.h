#ifndef RULEENGINEFACADE_H
#define RULEENGINEFACADE_H

#include "RuleEngine.h"
#include "CapacityRule.h"
#include "FeatureRule.h"
#include "StatusRule.h"
#include "LocationRule.h"

class RuleEngineFacade {
private:
    CapacityRule capacityRule;
    FeatureRule featureRule;
    StatusRule statusRule;
    LocationRule locationRule;
    RuleEngine ruleEngine;

public:
    RuleEngineFacade();

    RuleEvaluationResult evaluateRequest(OneTimeRequest& request,
                                         const std::vector<Allocation>& allocations) const;

    RuleEvaluationResult evaluateRequest(RecurringRequest& request,
                                         const std::vector<Allocation>& allocations) const;
};

#endif