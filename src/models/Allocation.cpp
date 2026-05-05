#include "Allocation.h"

Allocation::Allocation(int id, int requestId, const Space* space, const TimeSlot& timeSlot,
                       int assignedParticipants)
    : id(id),
      requestId(requestId),
      space(space),
      timeSlot(timeSlot),
      assignedParticipants(assignedParticipants),
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

int Allocation::getAssignedParticipants() const {
    return assignedParticipants;
}

AllocationStatus Allocation::getStatus() const {
    return status;
}
