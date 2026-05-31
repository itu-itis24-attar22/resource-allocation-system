#include "RequestParticipant.h"

RequestParticipant::RequestParticipant(int requestId,
                                       int userId,
                                       const std::string& participantRole)
    : requestId(requestId),
      userId(userId),
      participantRole(participantRole) {}

int RequestParticipant::getRequestId() const {
    return requestId;
}

int RequestParticipant::getUserId() const {
    return userId;
}

std::string RequestParticipant::getParticipantRole() const {
    return participantRole;
}
