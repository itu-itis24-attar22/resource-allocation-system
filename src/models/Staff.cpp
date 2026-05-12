#include "Staff.h"

Staff::Staff(int userId,
             const std::string& name,
             const std::string& email,
             const std::string& status,
             const std::string& jobTitle,
             const std::string& primaryUnitName,
             std::vector<std::string> assignedRoleNames)
    : User(userId, name, email, status, primaryUnitName, assignedRoleNames),
      jobTitle(jobTitle) {}

std::string Staff::getJobTitle() const {
    return jobTitle;
}

std::string Staff::getRoleName() const {
    return "Staff";
}

int Staff::getPriority() const {
    return 3;
}

bool Staff::canRequestSpaceType(const std::string& spaceType) const {
    return spaceType == "MeetingRoom";
}

bool Staff::canSubmitRequestType(const std::string& requestType) const {
    return requestType == "OneTime" ||
           requestType == "Recurring" ||
           requestType == "CommitteeMeeting" ||
           requestType == "Invalid";
}
