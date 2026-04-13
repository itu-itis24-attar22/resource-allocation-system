#include "RecurringRequest.h"

RecurringRequest::RecurringRequest(int requestId, const User& requester, Space* requestedSpace,
                                   const std::vector<TimeSlot>& requestedTimeSlots,
                                   int participantCount,
                                   const std::string& requiredFeature,
                                   const std::string& requiredBuilding)
    : Request(requestId, requester, requestedSpace, participantCount, requiredFeature, requiredBuilding),
      requestedTimeSlots(requestedTimeSlots) {}

const std::vector<TimeSlot>& RecurringRequest::getRequestedTimeSlots() const {
    return requestedTimeSlots;
}

bool RecurringRequest::isRecurring() const {
    return true;
}