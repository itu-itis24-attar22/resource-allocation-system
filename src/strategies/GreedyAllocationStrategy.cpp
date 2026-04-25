#include "GreedyAllocationStrategy.h"

namespace {
    bool hasSelfConflict(const std::vector<TimeSlot>& slots) {
        for (size_t i = 0; i < slots.size(); i++) {
            for (size_t j = i + 1; j < slots.size(); j++) {
                if (slots[i].overlapsWith(slots[j])) {
                    return true;
                }
            }
        }
        return false;
    }
}

bool GreedyAllocationStrategy::processRequest(OneTimeRequest& request,
                                              std::vector<Allocation>& allocations,
                                              const RuleEngineFacade& ruleEngineFacade) const {
    request.addHistoryEvent("evaluated");
    RuleEvaluationResult result = ruleEngineFacade.evaluateRequest(request, allocations);

    if (!result.isPassed()) {
        request.markRejected(result.getFailureReason());
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

bool GreedyAllocationStrategy::processRequest(RecurringRequest& request,
                                              std::vector<Allocation>& allocations,
                                              const RuleEngineFacade& ruleEngineFacade) const {
    request.addHistoryEvent("evaluated");

    if (hasSelfConflict(request.getRequestedTimeSlots())) {
        request.markRejected("Self-conflicting recurring request");
        return false;
    }

    RuleEvaluationResult result = ruleEngineFacade.evaluateRequest(request, allocations);

    if (!result.isPassed()) {
        request.markRejected(result.getFailureReason());
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
