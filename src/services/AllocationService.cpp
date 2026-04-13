#include <iostream>
#include "AllocationService.h"

const std::vector<Allocation>& AllocationService::getAllocations() const {
    return allocations;
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
        default: return "Unknown";
    }
}

void AllocationService::addExistingAllocation(const Allocation& allocation) {
    allocations.push_back(allocation);
}

bool AllocationService::evaluateCommonRules(Request& request) {
    if (!capacityRule.check(request)) {
        request.markRejected("Capacity insufficient");
        return false;
    }

    if (!featureRule.check(request)) {
        request.markRejected("Required feature missing");
        return false;
    }

    if (!statusRule.check(request)) {
        request.markRejected("Space under maintenance");
        return false;
    }

    if (!locationRule.check(request)) {
        request.markRejected("Required building mismatch");
        return false;
    }

    return true;
}

bool AllocationService::processRequest(OneTimeRequest& request) {
    if (!evaluateCommonRules(request)) {
        return false;
    }

    if (!availabilityRule.check(request, allocations)) {
        request.markRejected("Time slot unavailable");
        return false;
    }

    request.markApproved();

    int allocationId = static_cast<int>(allocations.size()) + 1;
    Allocation allocation(allocationId, request.getId(),
                          request.getRequestedSpace(),
                          request.getRequestedTimeSlot());
    allocations.push_back(allocation);

    return true;
}

bool AllocationService::processRequest(RecurringRequest& request) {
    if (!evaluateCommonRules(request)) {
        return false;
    }

    if (!availabilityRule.check(request, allocations)) {
        request.markRejected("Time slot unavailable");
        return false;
    }

    request.markApproved();

    const std::vector<TimeSlot>& slots = request.getRequestedTimeSlots();
    for (const TimeSlot& slot : slots) {
        int allocationId = static_cast<int>(allocations.size()) + 1;
        Allocation allocation(allocationId, request.getId(),
                              request.getRequestedSpace(), slot);
        allocations.push_back(allocation);
    }

    return true;
}

void AllocationService::printAllocations() const {
    std::cout << "\nCurrent Allocations:\n";
    std::cout << "-----------------------------------\n";

    for (const Allocation& allocation : allocations) {
        std::cout << "Allocation ID: " << allocation.getId() << "\n";
        std::cout << "Space: " << allocation.getSpace()->getName()
                  << " (" << allocation.getSpace()->getType() << ")\n";
        std::cout << "Building: " << allocation.getSpace()->getBuilding() << "\n";
        std::cout << "Request ID: " << allocation.getRequestId() << "\n";
        std::cout << "Time: "
                  << dayToString(allocation.getTimeSlot().getDay()) << ", "
                  << allocation.getTimeSlot().getStartHour() << ":00 - "
                  << allocation.getTimeSlot().getEndHour() << ":00\n";
        std::cout << "-----------------------------------\n";
    }
}