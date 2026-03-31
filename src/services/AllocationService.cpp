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

static std::string dayToString(int day) {
    switch (day) {
        case 1: return "Monday";
        case 2: return "Tuesday";
        case 3: return "Wednesday";
        case 4: return "Thursday";
        case 5: return "Friday";
        case 6: return "Saturday";
        case 7: return "Sunday";
        default: return "Unknown Day";
    }
}

void AllocationService::printAllocations() const {
    std::cout << "\nCurrent Allocations:\n";
    std::cout << "-----------------------------------\n";

    for (const Allocation& allocation : allocations) {
        std::cout << "Allocation ID: " << allocation.getId() << "\n";
        std::cout << "Space: " << allocation.getSpace()->getName()
                  << " (" << allocation.getSpace()->getType() << ")\n";
        std::cout << "Request ID: " << allocation.getRequestId() << "\n";
        std::cout << "Time: "
                  << dayToString(allocation.getTimeSlot().getDay()) << ", "
                  << allocation.getTimeSlot().getStartHour() << ":00 - "
                  << allocation.getTimeSlot().getEndHour() << ":00\n";
        std::cout << "-----------------------------------\n";
    }
}