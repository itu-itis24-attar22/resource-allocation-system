#ifndef MEETINGTIMESUGGESTION_H
#define MEETINGTIMESUGGESTION_H

#include <string>
#include "TimeSlot.h"

class MeetingTimeSuggestion {
private:
    TimeSlot suggestedTimeSlot;
    int spaceId;
    std::string reason;
    int score;
    int timeDistanceMinutes;
    int dayDistance;
    int conflictCount;

public:
    MeetingTimeSuggestion(const TimeSlot& suggestedTimeSlot,
                          int spaceId,
                          const std::string& reason,
                          int score = 0,
                          int timeDistanceMinutes = 0,
                          int dayDistance = 0,
                          int conflictCount = 0);

    TimeSlot getTimeSlot() const;
    int getSpaceId() const;
    std::string getReason() const;
    int getScore() const;
    int getTimeDistanceMinutes() const;
    int getDayDistance() const;
    int getConflictCount() const;
};

#endif
