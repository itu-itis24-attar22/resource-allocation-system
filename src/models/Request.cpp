#include "Request.h"
#include "UserRole.h"

Request::Request(int requestId, const User& requester, Space* requestedSpace,
                 int participantCount,
                 const std::string& requiredFeature,
                 const std::string& requiredBuilding)
    : requestId(requestId),
      requester(requester),
      requestedSpace(requestedSpace),
      participantCount(participantCount),
      priority(userRoleToPriority(requester.getRole())),
      status(RequestStatus::Pending),
      requiredFeature(requiredFeature),
      requiredBuilding(requiredBuilding),
      rejectionReason("") {}

int Request::getId() const {
    return requestId;
}

User Request::getRequester() const {
    return requester;
}

Space* Request::getRequestedSpace() const {
    return requestedSpace;
}

int Request::getParticipantCount() const {
    return participantCount;
}

int Request::getPriority() const {
    return priority;
}

RequestStatus Request::getStatus() const {
    return status;
}

std::string Request::getRequiredFeature() const {
    return requiredFeature;
}

std::string Request::getRequiredBuilding() const {
    return requiredBuilding;
}

std::string Request::getRejectionReason() const {
    return rejectionReason;
}

void Request::markApproved() {
    status = RequestStatus::Approved;
    rejectionReason = "";
}

void Request::markRejected(const std::string& reason) {
    status = RequestStatus::Rejected;
    rejectionReason = reason;
}