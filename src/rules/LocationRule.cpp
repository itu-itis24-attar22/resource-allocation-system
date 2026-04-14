#include "LocationRule.h"

RuleEvaluationResult LocationRule::evaluate(Request& request) const {
    if (request.getRequiredBuilding().empty()) {
        return RuleEvaluationResult(true, "");
    }

    if (request.getRequestedSpace()->getBuilding() == request.getRequiredBuilding()) {
        return RuleEvaluationResult(true, "");
    }

    return RuleEvaluationResult(false, "Required building mismatch");
}