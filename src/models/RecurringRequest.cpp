#include "RecurringRequest.h"

RecurringRequest::RecurringRequest(int requestId, const User& requester, Space* requestedSpace,
                                   const std::vector<TimeSlot>& requestedTimeSlots,
                                   int participantCount,
                                   const std::string& requiredFeature)
    : requestId(requestId),
      requester(requester),
      requestedSpace(requestedSpace),
      requestedTimeSlots(requestedTimeSlots),
      participantCount(participantCount),
      status(RequestStatus::Pending),
      requiredFeature(requiredFeature),
      rejectionReason("") {}

int RecurringRequest::getId() const {
    return requestId;
}

User RecurringRequest::getRequester() const {
    return requester;
}

Space* RecurringRequest::getRequestedSpace() const {
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

std::string RecurringRequest::getRequiredFeature() const {
    return requiredFeature;
}

std::string RecurringRequest::getRejectionReason() const {
    return rejectionReason;
}

void RecurringRequest::markApproved() {
    status = RequestStatus::Approved;
    rejectionReason = "";
}

void RecurringRequest::markRejected(const std::string& reason) {
    status = RequestStatus::Rejected;
    rejectionReason = reason;
}