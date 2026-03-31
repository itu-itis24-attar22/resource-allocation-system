#include <iostream>
#include "AllocationService.h"

AllocationService::AllocationService() : nextAllocationId(1) {}

void AllocationService::addExistingAllocation(const Allocation& allocation) {
    allocations.push_back(allocation);
}

bool AllocationService::processRequest(OneTimeRequest& request) {
    bool isAvailable = availabilityRule.check(request, allocations);
    bool hasEnoughCapacity = capacityRule.check(request);

    if (isAvailable && hasEnoughCapacity) {
        request.markApproved();

        Allocation newAllocation(
            nextAllocationId++,
            request.getId(),
            request.getRequestedSpace(),
            request.getRequestedTimeSlot()
        );

        allocations.push_back(newAllocation);
        return true;
    } else {
        request.markRejected();
        return false;
    }
}

bool AllocationService::processRequest(RecurringRequest& request) {
    bool isAvailable = availabilityRule.check(request, allocations);
    bool hasEnoughCapacity = capacityRule.check(request);

    if (isAvailable && hasEnoughCapacity) {
        request.markApproved();

        for (const auto& slot : request.getRequestedTimeSlots()) {
            Allocation newAllocation(
                nextAllocationId++,
                request.getId(),
                request.getRequestedSpace(),
                slot
            );
            allocations.push_back(newAllocation);
        }
        return true;
    } else {
        request.markRejected();
        return false;
    }
}

void AllocationService::printAllocations() const {
    std::cout << "\nCurrent Allocations:\n";

    for (const auto& allocation : allocations) {
        std::cout << "Allocation ID: " << allocation.getId()
                  << ", Type: " << allocation.getSpace()->getType()
                  << ", Space: " << allocation.getSpace()->getName()
                  << ", Time: " << allocation.getTimeSlot().getStartHour()
                  << ":00 - " << allocation.getTimeSlot().getEndHour()
                  << ":00\n";
    }
}