#include "Student.h"

Student::Student(int userId, const std::string& name)
    : User(userId, name) {}

std::string Student::getRoleName() const {
    return "Student";
}

int Student::getPriority() const {
    return 1;
}

bool Student::canRequestSpaceType(const std::string& spaceType) const {
    return spaceType == "Classroom" || spaceType == "MeetingRoom";
}

bool Student::canSubmitRequestType(const std::string& requestType) const {
    return requestType == "OneTime" ||
           requestType == "Recurring" ||
           requestType == "Invalid";
}
