#include "AvailabilityRule.h"

bool AvailabilityRule::isSpaceAvailable(const Space* space, const TimeSlot& timeSlot, const std::vector<Allocation>& existingAllocations) const {
    for (const auto& allocation : existingAllocations) {
        bool sameSpace = allocation.getSpace()->getId() == space->getId();
        bool overlapping = allocation.getTimeSlot().overlapsWith(timeSlot);

        if (sameSpace && overlapping) {
            return false;
        }
    }
    return true;
}

bool AvailabilityRule::check(const OneTimeRequest& request, const std::vector<Allocation>& existingAllocations) const {
    return isSpaceAvailable(request.getRequestedSpace(), request.getRequestedTimeSlot(), existingAllocations);
}

bool AvailabilityRule::check(const RecurringRequest& request, const std::vector<Allocation>& existingAllocations) const {
    for (const auto& slot : request.getRequestedTimeSlots()) {
        if (!isSpaceAvailable(request.getRequestedSpace(), slot, existingAllocations)) {
            return false;
        }
    }
    return true;
}

bool AvailabilityRule::check(const ExamRequest& request, const std::vector<Allocation>& existingAllocations) const {
    return isSpaceAvailable(request.getRequestedSpace(), request.getExamTimeSlot(), existingAllocations);
}
