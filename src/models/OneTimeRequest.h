#ifndef ONETIMEREQUEST_H
#define ONETIMEREQUEST_H

#include "User.h"
#include "Space.h"
#include "TimeSlot.h"
#include "Status.h"

class OneTimeRequest {
private:
    int id;
    User requester;
    const Space* requestedSpace;
    TimeSlot requestedTimeSlot;
    int participantCount;
    RequestStatus status;

public:
    OneTimeRequest(
        int id,
        const User& requester,
        const Space* space,
        const TimeSlot& timeSlot,
        int participantCount
    );

    int getId() const;
    const User& getRequester() const;
    const Space* getRequestedSpace() const;
    const TimeSlot& getRequestedTimeSlot() const;
    int getParticipantCount() const;
    RequestStatus getStatus() const;

    void markApproved();
    void markRejected();
};

#endif