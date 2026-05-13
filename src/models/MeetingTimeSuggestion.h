#ifndef MEETINGTIMESUGGESTION_H
#define MEETINGTIMESUGGESTION_H

#include <string>
#include "TimeSlot.h"

class MeetingTimeSuggestion {
private:
    TimeSlot suggestedTimeSlot;
    int spaceId;
    std::string reason;

public:
    MeetingTimeSuggestion(const TimeSlot& suggestedTimeSlot,
                          int spaceId,
                          const std::string& reason);

    TimeSlot getTimeSlot() const;
    int getSpaceId() const;
    std::string getReason() const;
};

#endif
