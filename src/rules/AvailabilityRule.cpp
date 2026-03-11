#include "AvailabilityRule.h"

bool AvailabilityRule::check(const OneTimeRequest& request, const std::vector<Allocation>& existingAllocations) const {
    for (const auto& allocation : existingAllocations) {
        bool sameSpace =
            allocation.getSpace()->getId() == request.getRequestedSpace()->getId();

        bool overlapping =
            allocation.getTimeSlot().overlapsWith(request.getRequestedTimeSlot());

        if (sameSpace && overlapping) {
            return false;
        }
    }
    return true;
}