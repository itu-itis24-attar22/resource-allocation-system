#include "StatusRule.h"

RuleEvaluationResult StatusRule::evaluate(Request& request) const {
    const Space* requestedSpace = request.getRequestedSpace();
    if (!requestedSpace) {
        return RuleEvaluationResult(false, "Requested space missing");
    }

    if (requestedSpace->getIsAvailable()) {
        return RuleEvaluationResult(true, "");
    }

    return RuleEvaluationResult(false, "Space under maintenance");
}
