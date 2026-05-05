#include "RequestTypeRule.h"

RuleEvaluationResult RequestTypeRule::evaluate(Request& request) const {
    const User* requester = request.getRequester();
    const std::string requestType = request.getRequestType();

    if (requester && requester->canSubmitRequestType(requestType)) {
        return RuleEvaluationResult(true, "");
    }

    return RuleEvaluationResult(false,
                                "User is not allowed to submit " + requestType + " requests");
}
