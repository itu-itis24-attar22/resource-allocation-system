#include "UserRoleRule.h"

RuleEvaluationResult UserRoleRule::evaluate(Request& request) const {
    const User* requester = request.getRequester();
    const Space* requestedSpace = request.getRequestedSpace();

    if (!requestedSpace) {
        return RuleEvaluationResult(false, "Requested space missing");
    }

    std::string spaceType = requestedSpace->getType();

    if (requester && requester->canRequestSpaceType(spaceType)) {
        return RuleEvaluationResult(true, "");
    }

    return RuleEvaluationResult(false, "User role not authorized for requested space type");
}
