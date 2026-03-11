#include "AvailabilityRule.h"

bool AvailabilityRule::check(const OneTimeRequest& request, const std::vector<Allocation>& existingAllocations) const {
    for (const auto& allocation : existingAllocations) {
        bool sameClassroom =
            allocation.getClassroom().getId() == request.getRequestedClassroom().getId();

        bool overlapping =
            allocation.getTimeSlot().overlapsWith(request.getRequestedTimeSlot());

        if (sameClassroom && overlapping) {
            return false;
        }
    }
    return true;
}