#include "StatusRule.h"

RuleEvaluationResult StatusRule::evaluate(Request& request) const {
    if (request.getRequestedSpace()->getIsAvailable()) {
        return RuleEvaluationResult(true, "");
    }

    return RuleEvaluationResult(false, "Space under maintenance");
}