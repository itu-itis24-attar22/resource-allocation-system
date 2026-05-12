#include "RuleEngineFacade.h"

RuleEngineFacade::RuleEngineFacade()
    : ruleEngine(std::vector<const IRequestRule*>{
          &requestTypeRule,
          &userRoleRule,
          &participantAvailabilityRule,
          &capacityRule,
          &featureRule,
          &statusRule,
          &locationRule
      }) {}

RuleEngineFacade::RuleEngineFacade(const std::vector<User*>& users,
                                   const std::vector<UserBusySlot>& userBusySlots)
    : participantAvailabilityRule(users, userBusySlots),
      ruleEngine(std::vector<const IRequestRule*>{
          &requestTypeRule,
          &userRoleRule,
          &participantAvailabilityRule,
          &capacityRule,
          &featureRule,
          &statusRule,
          &locationRule
      }) {}

RuleEvaluationResult RuleEngineFacade::evaluateRequest(OneTimeRequest& request,
                                                       const std::vector<Allocation>& allocations) const {
    return ruleEngine.evaluate(request, allocations);
}

RuleEvaluationResult RuleEngineFacade::evaluateRequest(RecurringRequest& request,
                                                       const std::vector<Allocation>& allocations) const {
    return ruleEngine.evaluate(request, allocations);
}

RuleEvaluationResult RuleEngineFacade::evaluateRequest(ExamRequest& request,
                                                       const std::vector<Allocation>& allocations) const {
    return ruleEngine.evaluate(request, allocations);
}

RuleEvaluationResult RuleEngineFacade::evaluateRequest(CommitteeMeetingRequest& request,
                                                       const std::vector<Allocation>& allocations) const {
    return ruleEngine.evaluate(request, allocations);
}
