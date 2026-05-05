#ifndef ALLOCATION_H
#define ALLOCATION_H

#include "Space.h"
#include "TimeSlot.h"
#include "Status.h"

class Allocation {
private:
    int id;
    int requestId;
    const Space* space;
    TimeSlot timeSlot;
    int assignedParticipants;
    AllocationStatus status;

public:
    Allocation(int id, int requestId, const Space* space, const TimeSlot& timeSlot,
               int assignedParticipants);

    int getId() const;
    int getRequestId() const;
    const Space* getSpace() const;
    const TimeSlot& getTimeSlot() const;
    int getAssignedParticipants() const;
    AllocationStatus getStatus() const;
};

#endif
