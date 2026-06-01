#include "CapacityRule.h"

RuleEvaluationResult CapacityRule::evaluate(Request& request) const {
    const Space* requestedSpace = request.getRequestedSpace();
    if (!requestedSpace) {
        return RuleEvaluationResult(false, "Requested space missing");
    }

    if (requestedSpace->getCapacity() >= request.getParticipantCount()) {
        return RuleEvaluationResult(true, "");
    }

    return RuleEvaluationResult(false, "Capacity insufficient");
}
