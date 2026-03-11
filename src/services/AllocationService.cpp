#include <iostream>
#include "AllocationService.h"

AllocationService::AllocationService() : nextAllocationId(1) {}

void AllocationService::addExistingAllocation(const Allocation& allocation) {
    allocations.push_back(allocation);
}

bool AllocationService::processRequest(OneTimeRequest& request) {
    bool available = rule.check(request, allocations);

    if (available) {
        request.markApproved();

        Allocation newAllocation(
            nextAllocationId++,
            request.getId(),
            request.getRequestedClassroom(),
            request.getRequestedTimeSlot()
        );

        allocations.push_back(newAllocation);
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
                  << ", Classroom: " << allocation.getClassroom().getName()
                  << ", Time: " << allocation.getTimeSlot().getStartHour()
                  << ":00 - " << allocation.getTimeSlot().getEndHour()
                  << ":00\n";
    }
}