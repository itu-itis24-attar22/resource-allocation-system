#include "Instructor.h"

Instructor::Instructor(int userId,
                       const std::string& name,
                       const std::string& email,
                       const std::string& status,
                       const std::string& title,
                       const std::string& officeRoom,
                       const std::string& primaryUnitName,
                       std::vector<std::string> assignedRoleNames)
    : User(userId, name, email, status, primaryUnitName, assignedRoleNames),
      title(title),
      officeRoom(officeRoom) {}

std::string Instructor::getTitle() const {
    return title;
}

std::string Instructor::getOfficeRoom() const {
    return officeRoom;
}

std::string Instructor::getRoleName() const {
    return "Instructor";
}

int Instructor::getPriority() const {
    return 4;
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
           requestType == "CommitteeMeeting" ||
           requestType == "Invalid";
}
