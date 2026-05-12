#ifndef RULEENGINEFACADE_H
#define RULEENGINEFACADE_H

#include "RuleEngine.h"
#include "CapacityRule.h"
#include "FeatureRule.h"
#include "StatusRule.h"
#include "LocationRule.h"
#include "UserRoleRule.h"
#include "RequestTypeRule.h"
#include "ParticipantAvailabilityRule.h"

class RuleEngineFacade {
private:
    RequestTypeRule requestTypeRule;
    ParticipantAvailabilityRule participantAvailabilityRule;
    CapacityRule capacityRule;
    FeatureRule featureRule;
    StatusRule statusRule;
    LocationRule locationRule;
    UserRoleRule userRoleRule;
    RuleEngine ruleEngine;

public:
    RuleEngineFacade();
    RuleEngineFacade(const std::vector<User*>& users,
                     const std::vector<UserBusySlot>& userBusySlots);

    RuleEvaluationResult evaluateRequest(OneTimeRequest& request,
                                         const std::vector<Allocation>& allocations) const;

    RuleEvaluationResult evaluateRequest(RecurringRequest& request,
                                         const std::vector<Allocation>& allocations) const;

    RuleEvaluationResult evaluateRequest(ExamRequest& request,
                                         const std::vector<Allocation>& allocations) const;

    RuleEvaluationResult evaluateRequest(CommitteeMeetingRequest& request,
                                         const std::vector<Allocation>& allocations) const;
};

#endif
