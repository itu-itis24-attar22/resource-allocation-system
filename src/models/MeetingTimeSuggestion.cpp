#include "MeetingTimeSuggestion.h"

MeetingTimeSuggestion::MeetingTimeSuggestion(
    const TimeSlot& suggestedTimeSlot,
    int spaceId,
    const std::string& reason
)
    : suggestedTimeSlot(suggestedTimeSlot),
      spaceId(spaceId),
      reason(reason) {}

TimeSlot MeetingTimeSuggestion::getTimeSlot() const {
    return suggestedTimeSlot;
}

int MeetingTimeSuggestion::getSpaceId() const {
    return spaceId;
}

std::string MeetingTimeSuggestion::getReason() const {
    return reason;
}
