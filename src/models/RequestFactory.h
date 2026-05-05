#ifndef REQUESTFACTORY_H
#define REQUESTFACTORY_H

#include <string>
#include "Request.h"

class RequestFactory {
public:
    static Request* createRequest(int requestId,
                                  const std::string& requestType,
                                  User* requester,
                                  Space* requestedSpace,
                                  int participantCount,
                                  const std::string& title,
                                  const std::string& purpose,
                                  const std::string& requiredFeature,
                                  const std::string& requiredBuilding,
                                  const std::string& timeData);

    static Request* createInvalidRequest(int requestId,
                                         const std::string& requestType,
                                         User* requester,
                                         Space* requestedSpace,
                                         int participantCount,
                                         const std::string& title,
                                         const std::string& purpose,
                                         const std::string& requiredFeature,
                                         const std::string& requiredBuilding,
                                         const std::string& timeData,
                                         const std::string& rejectionReason);
};

#endif
