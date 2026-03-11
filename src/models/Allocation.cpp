#include "Allocation.h"

Allocation::Allocation(int id, int requestId, const Classroom& classroom, const TimeSlot& timeSlot)
    : id(id),
      requestId(requestId),
      classroom(classroom),
      timeSlot(timeSlot),
      status(AllocationStatus::Active) {}

int Allocation::getId() const {
    return id;
}

int Allocation::getRequestId() const {
    return requestId;
}

const Classroom& Allocation::getClassroom() const {
    return classroom;
}

const TimeSlot& Allocation::getTimeSlot() const {
    return timeSlot;
}

AllocationStatus Allocation::getStatus() const {
    return status;
}