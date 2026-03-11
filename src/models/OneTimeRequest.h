#ifndef ONETIMEREQUEST_H
#define ONETIMEREQUEST_H

#include "User.h"
#include "Classroom.h"
#include "TimeSlot.h"
#include "Status.h"

class OneTimeRequest {
private:
    int id;
    User requester;
    Classroom requestedClassroom;
    TimeSlot requestedTimeSlot;
    int participantCount;
    RequestStatus status;

public:
    OneTimeRequest(int id, const User& requester, const Classroom& classroom, const TimeSlot& timeSlot, int participantCount);

    int getId() const;
    const User& getRequester() const;
    const Classroom& getRequestedClassroom() const;
    const TimeSlot& getRequestedTimeSlot() const;
    int getParticipantCount() const;
    RequestStatus getStatus() const;

    void markApproved();
    void markRejected();
};

#endif