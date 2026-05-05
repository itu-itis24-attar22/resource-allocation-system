#include "InvalidRequest.h"

InvalidRequest::InvalidRequest(int requestId, User* requester, Space* requestedSpace,
                               int participantCount,
                               const std::string& requestTypeLabel,
                               const std::string& rawTimeInfo,
                               const std::string& title,
                               const std::string& purpose,
                               const std::string& requiredFeature,
                               const std::string& requiredBuilding)
    : Request(requestId, requester, requestedSpace, participantCount, title, purpose,
              requiredFeature, requiredBuilding),
      requestTypeLabel(requestTypeLabel),
      rawTimeInfo(rawTimeInfo) {}

std::string InvalidRequest::getRequestTypeLabel() const {
    return requestTypeLabel;
}

std::string InvalidRequest::getRawTimeInfo() const {
    return rawTimeInfo;
}

bool InvalidRequest::isRecurring() const {
    return false;
}
