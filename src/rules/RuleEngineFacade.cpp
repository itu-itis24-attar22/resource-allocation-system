#include "RuleEngineFacade.h"

RuleEngineFacade::RuleEngineFacade()
    : ruleEngine(std::vector<const IRequestRule*>{
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