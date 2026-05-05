#include "OneTimeRequest.h"

OneTimeRequest::OneTimeRequest(int requestId, User* requester, Space* requestedSpace,
                               const TimeSlot& requestedTimeSlot, int participantCount,
                               const std::string& title,
                               const std::string& purpose,
                               const std::string& requiredFeature,
                               const std::string& requiredBuilding)
    : Request(requestId, requester, requestedSpace, participantCount, title, purpose,
              requiredFeature, requiredBuilding),
      requestedTimeSlot(requestedTimeSlot) {}

TimeSlot OneTimeRequest::getRequestedTimeSlot() const {
    return requestedTimeSlot;
}

bool OneTimeRequest::isRecurring() const {
    return false;
}

std::string OneTimeRequest::getRequestType() const {
    return "OneTime";
}
