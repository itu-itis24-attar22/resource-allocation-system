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
    AllocationStatus status;

public:
    Allocation(int id, int requestId, const Space* space, const TimeSlot& timeSlot);

    int getId() const;
    int getRequestId() const;
    const Space* getSpace() const;
    const TimeSlot& getTimeSlot() const;
    AllocationStatus getStatus() const;
};

#endif