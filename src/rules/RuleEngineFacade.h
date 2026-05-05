#ifndef RULEENGINEFACADE_H
#define RULEENGINEFACADE_H

#include "RuleEngine.h"
#include "CapacityRule.h"
#include "FeatureRule.h"
#include "StatusRule.h"
#include "LocationRule.h"
#include "UserRoleRule.h"
#include "RequestTypeRule.h"

class RuleEngineFacade {
private:
    RequestTypeRule requestTypeRule;
    CapacityRule capacityRule;
    FeatureRule featureRule;
    StatusRule statusRule;
    LocationRule locationRule;
    UserRoleRule userRoleRule;
    RuleEngine ruleEngine;

public:
    RuleEngineFacade();

    RuleEvaluationResult evaluateRequest(OneTimeRequest& request,
                                         const std::vector<Allocation>& allocations) const;

    RuleEvaluationResult evaluateRequest(RecurringRequest& request,
                                         const std::vector<Allocation>& allocations) const;

    RuleEvaluationResult evaluateRequest(ExamRequest& request,
                                         const std::vector<Allocation>& allocations) const;
};

#endif
