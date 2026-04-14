#include "CapacityRule.h"

RuleEvaluationResult CapacityRule::evaluate(Request& request) const {
    if (request.getRequestedSpace()->getCapacity() >= request.getParticipantCount()) {
        return RuleEvaluationResult(true, "");
    }

    return RuleEvaluationResult(false, "Capacity insufficient");
}