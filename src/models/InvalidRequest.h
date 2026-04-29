#ifndef INVALIDREQUEST_H
#define INVALIDREQUEST_H

#include "Request.h"

class InvalidRequest : public Request {
private:
    std::string requestTypeLabel;
    std::string rawTimeInfo;

public:
    InvalidRequest(int requestId, User* requester, Space* requestedSpace,
                   int participantCount,
                   const std::string& requestTypeLabel,
                   const std::string& rawTimeInfo,
                   const std::string& requiredFeature = "",
                   const std::string& requiredBuilding = "");

    std::string getRequestTypeLabel() const;
    std::string getRawTimeInfo() const;

    bool isRecurring() const override;
};

#endif
