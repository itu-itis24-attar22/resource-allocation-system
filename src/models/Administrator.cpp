#include "Administrator.h"

Administrator::Administrator(int userId, const std::string& name)
    : User(userId, name) {}

std::string Administrator::getRoleName() const {
    return "Administrator";
}

int Administrator::getPriority() const {
    return 4;
}

bool Administrator::canRequestSpaceType(const std::string& spaceType) const {
    return spaceType == "Classroom" ||
           spaceType == "Laboratory" ||
           spaceType == "MeetingRoom";
}

bool Administrator::canSubmitRequestType(const std::string& requestType) const {
    return requestType == "OneTime" ||
           requestType == "Recurring" ||
           requestType == "Exam" ||
           requestType == "Invalid";
}
