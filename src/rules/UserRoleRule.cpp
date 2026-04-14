#include "UserRoleRule.h"
#include "../models/UserRole.h"

RuleEvaluationResult UserRoleRule::evaluate(Request& request) const {
    UserRole role = request.getRequester().getRole();
    std::string spaceType = request.getRequestedSpace()->getType();

    switch (role) {
        case UserRole::Student:
            if (spaceType == "Classroom" || spaceType == "MeetingRoom") {
                return RuleEvaluationResult(true, "");
            }
            break;

        case UserRole::Instructor:
            if (spaceType == "Classroom" || spaceType == "Laboratory" || spaceType == "MeetingRoom") {
                return RuleEvaluationResult(true, "");
            }
            break;

        case UserRole::Staff:
            if (spaceType == "MeetingRoom") {
                return RuleEvaluationResult(true, "");
            }
            break;

        case UserRole::Administrator:
            return RuleEvaluationResult(true, "");
    }

    return RuleEvaluationResult(false, "User role not authorized for requested space type");
}