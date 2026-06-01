#include "LocationRule.h"

RuleEvaluationResult LocationRule::evaluate(Request& request) const {
    if (request.getRequiredBuilding().empty()) {
        return RuleEvaluationResult(true, "");
    }

    const Space* requestedSpace = request.getRequestedSpace();
    if (!requestedSpace) {
        return RuleEvaluationResult(false, "Requested space missing");
    }

    if (requestedSpace->getBuilding() == request.getRequiredBuilding()) {
        return RuleEvaluationResult(true, "");
    }

    return RuleEvaluationResult(false, "Required building mismatch");
}
