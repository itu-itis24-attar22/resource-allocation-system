#include "FeatureRule.h"

RuleEvaluationResult FeatureRule::evaluate(Request& request) const {
    const Space* requestedSpace = request.getRequestedSpace();
    if (!requestedSpace) {
        return RuleEvaluationResult(false, "Requested space missing");
    }

    if (requestedSpace->hasFeature(request.getRequiredFeature())) {
        return RuleEvaluationResult(true, "");
    }

    return RuleEvaluationResult(false, "Required feature missing");
}
