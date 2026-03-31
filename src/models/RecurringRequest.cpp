#include "RecurringRequest.h"

RecurringRequest::RecurringRequest(
    int id,
    const User& requester,
    const Space* space,
    const std::vector<TimeSlot>& timeSlots,
    int participantCount
)
    : id(id),
      requester(requester),
      requestedSpace(space),
      requestedTimeSlots(timeSlots),
      participantCount(participantCount),
      status(RequestStatus::Pending) {}

int RecurringRequest::getId() const {
    return id;
}

const User& RecurringRequest::getRequester() const {
    return requester;
}

const Space* RecurringRequest::getRequestedSpace() const {
    return requestedSpace;
}

const std::vector<TimeSlot>& RecurringRequest::getRequestedTimeSlots() const {
    return requestedTimeSlots;
}

int RecurringRequest::getParticipantCount() const {
    return participantCount;
}

RequestStatus RecurringRequest::getStatus() const {
    return status;
}

void RecurringRequest::markApproved() {
    status = RequestStatus::Approved;
}

void RecurringRequest::markRejected() {
    status = RequestStatus::Rejected;
}