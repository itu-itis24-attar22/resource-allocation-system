#include "FeatureRule.h"

RuleEvaluationResult FeatureRule::evaluate(Request& request) const {
    if (request.getRequestedSpace()->hasFeature(request.getRequiredFeature())) {
        return RuleEvaluationResult(true, "");
    }

    return RuleEvaluationResult(false, "Required feature missing");
}