#include "Allocation.h"

Allocation::Allocation(int id, int requestId, const Space* space, const TimeSlot& timeSlot)
    : id(id),
      requestId(requestId),
      space(space),
      timeSlot(timeSlot),
      status(AllocationStatus::Active) {}

int Allocation::getId() const {
    return id;
}

int Allocation::getRequestId() const {
    return requestId;
}

const Space* Allocation::getSpace() const {
    return space;
}

const TimeSlot& Allocation::getTimeSlot() const {
    return timeSlot;
}

AllocationStatus Allocation::getStatus() const {
    return status;
}