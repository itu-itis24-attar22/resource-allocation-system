#include "OneTimeRequest.h"

OneTimeRequest::OneTimeRequest(int id, const User& requester, const Classroom& classroom, const TimeSlot& timeSlot, int participantCount)
    : id(id),
      requester(requester),
      requestedClassroom(classroom),
      requestedTimeSlot(timeSlot),
      participantCount(participantCount),
      status(RequestStatus::Pending) {}

int OneTimeRequest::getId() const {
    return id;
}

const User& OneTimeRequest::getRequester() const {
    return requester;
}

const Classroom& OneTimeRequest::getRequestedClassroom() const {
    return requestedClassroom;
}

int OneTimeRequest::getParticipantCount() const {
    return participantCount;
}

const TimeSlot& OneTimeRequest::getRequestedTimeSlot() const {
    return requestedTimeSlot;
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