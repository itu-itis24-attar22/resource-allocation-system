#include "OneTimeRequest.h"

OneTimeRequest::OneTimeRequest(
    int id,
    const User& requester,
    const Space* space,
    const TimeSlot& timeSlot,
    int participantCount
)
    : id(id),
      requester(requester),
      requestedSpace(space),
      requestedTimeSlot(timeSlot),
      participantCount(participantCount),
      status(RequestStatus::Pending) {}

int OneTimeRequest::getId() const {
    return id;
}

const User& OneTimeRequest::getRequester() const {
    return requester;
}

const Space* OneTimeRequest::getRequestedSpace() const {
    return requestedSpace;
}

const TimeSlot& OneTimeRequest::getRequestedTimeSlot() const {
    return requestedTimeSlot;
}

int OneTimeRequest::getParticipantCount() const {
    return participantCount;
}

RequestStatus OneTimeRequest::getStatus() const {
    return status;
}

void OneTimeRequest::markApproved() {
    status = RequestStatus::Approved;
}

void OneTimeRequest::markRejected() {
    status = RequestStatus::Rejected;
}