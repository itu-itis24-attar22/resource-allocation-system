#include "Student.h"

Student::Student(int userId,
                 const std::string& name,
                 const std::string& email,
                 const std::string& status,
                 const std::string& studentNo,
                 const std::string& program,
                 int yearLevel,
                 const std::string& primaryUnitName,
                 std::vector<std::string> assignedRoleNames)
    : User(userId, name, email, status, primaryUnitName, assignedRoleNames),
      studentNo(studentNo),
      program(program),
      yearLevel(yearLevel) {}

std::string Student::getStudentNo() const {
    return studentNo;
}

std::string Student::getProgram() const {
    return program;
}

int Student::getYearLevel() const {
    return yearLevel;
}

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
           requestType == "CommitteeMeeting" ||
           requestType == "Invalid";
}
