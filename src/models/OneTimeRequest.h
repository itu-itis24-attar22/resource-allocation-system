#ifndef ONETIMEREQUEST_H
#define ONETIMEREQUEST_H

#include <string>
#include "User.h"
#include "Space.h"
#include "TimeSlot.h"
#include "Status.h"

class OneTimeRequest {
private:
    int requestId;
    User requester;
    Space* requestedSpace;
    TimeSlot requestedTimeSlot;
    int participantCount;
    RequestStatus status;
    std::string requiredFeature;
    std::string rejectionReason;

public:
    OneTimeRequest(int requestId, const User& requester, Space* requestedSpace,
                   const TimeSlot& requestedTimeSlot, int participantCount,
                   const std::string& requiredFeature = "");

    int getId() const;
    User getRequester() const;
    Space* getRequestedSpace() const;
    TimeSlot getRequestedTimeSlot() const;
    int getParticipantCount() const;
    RequestStatus getStatus() const;
    std::string getRequiredFeature() const;
    std::string getRejectionReason() const;

    void markApproved();
    void markRejected(const std::string& reason);
};

#endif