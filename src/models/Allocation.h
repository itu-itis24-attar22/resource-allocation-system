#ifndef ALLOCATION_H
#define ALLOCATION_H

#include "Classroom.h"
#include "TimeSlot.h"
#include "Status.h"

class Allocation {
private:
    int id;
    int requestId;
    Classroom classroom;
    TimeSlot timeSlot;
    AllocationStatus status;

public:
    Allocation(int id, int requestId, const Classroom& classroom, const TimeSlot& timeSlot);

    int getId() const;
    int getRequestId() const;
    const Classroom& getClassroom() const;
    const TimeSlot& getTimeSlot() const;
    AllocationStatus getStatus() const;
};

#endif