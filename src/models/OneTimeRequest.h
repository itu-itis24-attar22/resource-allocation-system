#ifndef ONETIMEREQUEST_H
#define ONETIMEREQUEST_H

#include "Request.h"
#include "TimeSlot.h"

class OneTimeRequest : public Request {
private:
    TimeSlot requestedTimeSlot;

public:
    OneTimeRequest(int requestId, const User& requester, Space* requestedSpace,
                   const TimeSlot& requestedTimeSlot, int participantCount,
                   const std::string& requiredFeature = "",
                   const std::string& requiredBuilding = "");

    TimeSlot getRequestedTimeSlot() const;

    bool isRecurring() const override;
};

#endif