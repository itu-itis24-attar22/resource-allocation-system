#ifndef USERBUSYSLOT_H
#define USERBUSYSLOT_H

#include <string>
#include "TimeSlot.h"

class UserBusySlot {
private:
    int userId;
    TimeSlot timeSlot;
    std::string reason;

public:
    UserBusySlot(int userId, const TimeSlot& timeSlot, const std::string& reason = "");

    int getUserId() const;
    const TimeSlot& getTimeSlot() const;
    std::string getReason() const;
};

#endif
