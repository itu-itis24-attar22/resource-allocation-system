#include "MeetingTimeSuggestion.h"

MeetingTimeSuggestion::MeetingTimeSuggestion(
    const TimeSlot& suggestedTimeSlot,
    int spaceId,
    const std::string& reason,
    int score,
    int timeDistanceMinutes,
    int dayDistance,
    int conflictCount
)
    : suggestedTimeSlot(suggestedTimeSlot),
      spaceId(spaceId),
      reason(reason),
      score(score),
      timeDistanceMinutes(timeDistanceMinutes),
      dayDistance(dayDistance),
      conflictCount(conflictCount) {}

TimeSlot MeetingTimeSuggestion::getTimeSlot() const {
    return suggestedTimeSlot;
}

int MeetingTimeSuggestion::getSpaceId() const {
    return spaceId;
}

std::string MeetingTimeSuggestion::getReason() const {
    return reason;
}

int MeetingTimeSuggestion::getScore() const {
    return score;
}

int MeetingTimeSuggestion::getTimeDistanceMinutes() const {
    return timeDistanceMinutes;
}

int MeetingTimeSuggestion::getDayDistance() const {
    return dayDistance;
}

int MeetingTimeSuggestion::getConflictCount() const {
    return conflictCount;
}
