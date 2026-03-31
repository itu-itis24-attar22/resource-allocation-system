#include "OneTimeRequest.h"

OneTimeRequest::OneTimeRequest(int requestId, const User& requester, Space* requestedSpace,
                               const TimeSlot& requestedTimeSlot, int participantCount,
                               const std::string& requiredFeature)
    : requestId(requestId),
      requester(requester),
      requestedSpace(requestedSpace),
      requestedTimeSlot(requestedTimeSlot),
      participantCount(participantCount),
      status(RequestStatus::Pending),
      requiredFeature(requiredFeature),
      rejectionReason("") {}

int OneTimeRequest::getId() const {
    return requestId;
}

User OneTimeRequest::getRequester() const {
    return requester;
}

Space* OneTimeRequest::getRequestedSpace() const {
    return requestedSpace;
}

TimeSlot OneTimeRequest::getRequestedTimeSlot() const {
    return requestedTimeSlot;
}

int OneTimeRequest::getParticipantCount() const {
    return participantCount;
}

RequestStatus OneTimeRequest::getStatus() const {
    return status;
}

std::string OneTimeRequest::getRequiredFeature() const {
    return requiredFeature;
}

std::string OneTimeRequest::getRejectionReason() const {
    return rejectionReason;
}

void OneTimeRequest::markApproved() {
    status = RequestStatus::Approved;
    rejectionReason = "";
}

void OneTimeRequest::markRejected(const std::string& reason) {
    status = RequestStatus::Rejected;
    rejectionReason = reason;
}