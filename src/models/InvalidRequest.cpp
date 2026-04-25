#include "InvalidRequest.h"

InvalidRequest::InvalidRequest(int requestId, const User& requester, Space* requestedSpace,
                               int participantCount,
                               const std::string& requestTypeLabel,
                               const std::string& rawTimeInfo,
                               const std::string& requiredFeature,
                               const std::string& requiredBuilding)
    : Request(requestId, requester, requestedSpace, participantCount, requiredFeature, requiredBuilding),
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
