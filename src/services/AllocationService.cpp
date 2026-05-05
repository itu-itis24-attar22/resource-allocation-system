#include <iostream>
#include "AllocationService.h"
#include "../strategies/AllocationStrategyFactory.h"

AllocationService::AllocationService()
    : AllocationService("greedy") {}

AllocationService::AllocationService(const std::string& strategyName)
    : allocationStrategy(AllocationStrategyFactory::getInstance().getStrategy(strategyName)) {}

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

void AllocationService::processRequests(const std::vector<Request*>& requests) {
    allocationStrategy->processRequests(requests, allocations, ruleEngineFacade);
}

bool AllocationService::processRequest(OneTimeRequest& request) {
    return allocationStrategy->processRequest(request, allocations, ruleEngineFacade);
}

bool AllocationService::processRequest(RecurringRequest& request) {
    return allocationStrategy->processRequest(request, allocations, ruleEngineFacade);
}

bool AllocationService::processRequest(ExamRequest& request) {
    return allocationStrategy->processRequest(request, allocations, ruleEngineFacade);
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
        std::cout << "Assigned participants: " << allocation.getAssignedParticipants() << "\n";
        std::cout << "Time: "
                  << dayToString(allocation.getTimeSlot().getDay()) << ", "
                  << allocation.getTimeSlot().getStartHour() << ":00 - "
                  << allocation.getTimeSlot().getEndHour() << ":00\n";
        std::cout << "-----------------------------------\n";
    }
}
