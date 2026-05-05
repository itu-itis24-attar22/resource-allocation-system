#include "Instructor.h"

Instructor::Instructor(int userId, const std::string& name)
    : User(userId, name) {}

std::string Instructor::getRoleName() const {
    return "Instructor";
}

int Instructor::getPriority() const {
    return 3;
}

bool Instructor::canRequestSpaceType(const std::string& spaceType) const {
    return spaceType == "Classroom" ||
           spaceType == "Laboratory" ||
           spaceType == "MeetingRoom";
}

bool Instructor::canSubmitRequestType(const std::string& requestType) const {
    return requestType == "OneTime" ||
           requestType == "Recurring" ||
           requestType == "Exam" ||
           requestType == "Invalid";
}
