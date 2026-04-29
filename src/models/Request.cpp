#include "Request.h"

Request::Request(int requestId, User* requester, Space* requestedSpace,
                 int participantCount,
                 const std::string& requiredFeature,
                 const std::string& requiredBuilding)
    : requestId(requestId),
      requester(requester),
      requestedSpace(requestedSpace),
      participantCount(participantCount),
      priority(requester ? requester->getPriority() : 0),
      status(RequestStatus::Pending),
      requiredFeature(requiredFeature),
      requiredBuilding(requiredBuilding),
      rejectionReason("") {
    addHistoryEvent("created");
}

int Request::getId() const {
    return requestId;
}

const User* Request::getRequester() const {
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

const std::vector<std::string>& Request::getLifecycleHistory() const {
    return lifecycleHistory;
}

void Request::addHistoryEvent(const std::string& event) {
    lifecycleHistory.push_back(event);
}

void Request::markApproved() {
    status = RequestStatus::Approved;
    rejectionReason = "";
    addHistoryEvent("approved");
}

void Request::markRejected(const std::string& reason) {
    status = RequestStatus::Rejected;
    rejectionReason = reason;
    addHistoryEvent("rejected: " + reason);
}
