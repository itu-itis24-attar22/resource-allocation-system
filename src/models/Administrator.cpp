#include "Administrator.h"

Administrator::Administrator(int userId,
                             const std::string& name,
                             const std::string& email,
                             const std::string& status,
                             const std::string& adminLevel,
                             const std::string& primaryUnitName,
                             std::vector<std::string> assignedRoleNames)
    : User(userId, name, email, status, primaryUnitName, assignedRoleNames),
      adminLevel(adminLevel) {}

std::string Administrator::getAdminLevel() const {
    return adminLevel;
}

std::string Administrator::getRoleName() const {
    return "Administrator";
}

int Administrator::getPriority() const {
    return 5;
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
