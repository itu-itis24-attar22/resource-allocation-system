#include "OneTimeRequest.h"

OneTimeRequest::OneTimeRequest(int requestId, User* requester, Space* requestedSpace,
                               const TimeSlot& requestedTimeSlot, int participantCount,
                               const std::string& requiredFeature,
                               const std::string& requiredBuilding)
    : Request(requestId, requester, requestedSpace, participantCount, requiredFeature, requiredBuilding),
      requestedTimeSlot(requestedTimeSlot) {}

TimeSlot OneTimeRequest::getRequestedTimeSlot() const {
    return requestedTimeSlot;
}

bool OneTimeRequest::isRecurring() const {
    return false;
}
