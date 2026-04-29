#include "Staff.h"

Staff::Staff(int userId, const std::string& name)
    : User(userId, name) {}

std::string Staff::getRoleName() const {
    return "Staff";
}

int Staff::getPriority() const {
    return 2;
}

bool Staff::canRequestSpaceType(const std::string& spaceType) const {
    return spaceType == "MeetingRoom";
}
