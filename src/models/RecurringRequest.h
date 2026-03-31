#ifndef RECURRINGREQUEST_H
#define RECURRINGREQUEST_H

#include <vector>
#include "User.h"
#include "Space.h"
#include "TimeSlot.h"
#include "Status.h"

class RecurringRequest {
private:
    int id;
    User requester;
    const Space* requestedSpace;
    std::vector<TimeSlot> requestedTimeSlots;
    int participantCount;
    RequestStatus status;

public:
    RecurringRequest(
        int id,
        const User& requester,
        const Space* space,
        const std::vector<TimeSlot>& timeSlots,
        int participantCount
    );

    int getId() const;
    const User& getRequester() const;
    const Space* getRequestedSpace() const;
    const std::vector<TimeSlot>& getRequestedTimeSlots() const;
    int getParticipantCount() const;
    RequestStatus getStatus() const;

    void markApproved();
    void markRejected();
};

#endif