#include "UserBusySlot.h"

UserBusySlot::UserBusySlot(int userId, const TimeSlot& timeSlot, const std::string& reason)
    : userId(userId), timeSlot(timeSlot), reason(reason) {}

int UserBusySlot::getUserId() const {
    return userId;
}

const TimeSlot& UserBusySlot::getTimeSlot() const {
    return timeSlot;
}

std::string UserBusySlot::getReason() const {
    return reason;
}
