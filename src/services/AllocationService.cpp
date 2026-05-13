#include <iostream>
#include <sstream>
#include "AllocationService.h"
#include "../strategies/AllocationStrategyFactory.h"

AllocationService::AllocationService()
    : AllocationService("greedy") {}

AllocationService::AllocationService(const std::string& strategyName)
    : userBusySlots(),
      meetingTimeSuggestionService(),
      ruleEngineFacade(),
      allocationStrategy(AllocationStrategyFactory::getInstance().getStrategy(strategyName)) {}

AllocationService::AllocationService(
    const std::string& strategyName,
    const std::vector<User*>& users,
    const std::vector<UserBusySlot>& userBusySlots
)
    : userBusySlots(userBusySlots),
      meetingTimeSuggestionService(),
      ruleEngineFacade(users, userBusySlots),
      allocationStrategy(AllocationStrategyFactory::getInstance().getStrategy(strategyName)) {}

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
    const std::vector<Space*> emptySpaces;
    processRequests(requests, emptySpaces);
}

void AllocationService::processRequests(const std::vector<Request*>& requests,
                                        const std::vector<Space*>& spaces) {
    allocationStrategy->processRequests(requests, spaces, allocations, ruleEngineFacade);
    appendMeetingTimeSuggestionsForRejectedCommitteeRequests(requests);
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

bool AllocationService::processRequest(CommitteeMeetingRequest& request) {
    const bool approved =
        allocationStrategy->processRequest(request, allocations, ruleEngineFacade);

    if (!approved) {
        appendMeetingTimeSuggestions(request);
    }

    return approved;
}

void AllocationService::appendMeetingTimeSuggestions(
    CommitteeMeetingRequest& request
) const {
    if (request.getStatus() != RequestStatus::Rejected) {
        return;
    }

    const std::vector<MeetingTimeSuggestion> suggestions =
        meetingTimeSuggestionService.suggestTimes(
            request,
            request.getRequestedSpace(),
            userBusySlots,
            allocations,
            3
        );

    if (suggestions.empty()) {
        request.addHistoryEvent("No available alternative time found.");
        return;
    }

    std::ostringstream message;
    message << "Suggested least-change alternative times: ";

    for (size_t i = 0; i < suggestions.size(); ++i) {
        TimeSlot slot = suggestions[i].getTimeSlot();
        message << (i + 1) << ") "
                << dayToString(slot.getDay()) << " "
                << slot.getStartTimeString() << "-"
                << slot.getEndTimeString()
                << " in space " << suggestions[i].getSpaceId()
                << " (distance: "
                << suggestions[i].getTimeDistanceMinutes()
                << " minutes)";

        if (i + 1 < suggestions.size()) {
            message << "; ";
        }
    }

    request.addHistoryEvent(message.str());
}

void AllocationService::appendMeetingTimeSuggestionsForRejectedCommitteeRequests(
    const std::vector<Request*>& requests
) const {
    for (Request* request : requests) {
        CommitteeMeetingRequest* committeeRequest =
            dynamic_cast<CommitteeMeetingRequest*>(request);

        if (committeeRequest &&
            committeeRequest->getStatus() == RequestStatus::Rejected) {
            appendMeetingTimeSuggestions(*committeeRequest);
        }
    }
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
                  << allocation.getTimeSlot().getStartTimeString() << " - "
                  << allocation.getTimeSlot().getEndTimeString() << "\n";
        std::cout << "-----------------------------------\n";
    }
}
