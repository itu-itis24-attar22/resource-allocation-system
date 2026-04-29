#include "UserRoleRule.h"

RuleEvaluationResult UserRoleRule::evaluate(Request& request) const {
    const User* requester = request.getRequester();
    std::string spaceType = request.getRequestedSpace()->getType();

    if (requester && requester->canRequestSpaceType(spaceType)) {
        return RuleEvaluationResult(true, "");
    }

    return RuleEvaluationResult(false, "User role not authorized for requested space type");
}
