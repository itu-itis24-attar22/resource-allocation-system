#include "TeachingAssistant.h"

TeachingAssistant::TeachingAssistant(int userId,
                                     const std::string& name,
                                     const std::string& email,
                                     const std::string& status,
                                     const std::string& assistantType,
                                     const std::string& primaryUnitName,
                                     std::vector<std::string> assignedRoleNames)
    : User(userId, name, email, status, primaryUnitName, assignedRoleNames),
      assistantType(assistantType) {}

std::string TeachingAssistant::getAssistantType() const {
    return assistantType;
}

std::string TeachingAssistant::getRoleName() const {
    return "TeachingAssistant";
}

int TeachingAssistant::getPriority() const {
    return 2;
}

bool TeachingAssistant::canRequestSpaceType(const std::string& spaceType) const {
    return spaceType == "Classroom" ||
           spaceType == "Laboratory" ||
           spaceType == "MeetingRoom";
}

bool TeachingAssistant::canSubmitRequestType(const std::string& requestType) const {
    return requestType == "OneTime" ||
           requestType == "Recurring" ||
           requestType == "CommitteeMeeting" ||
           requestType == "Invalid";
}
