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

    int nextAllocationId(const std::vector<Allocation>& allocations) {
        return static_cast<int>(allocations.size()) + 1;
    }
}

void GreedyAllocationStrategy::processRequests(const std::vector<Request*>& requests,
                                               std::vector<Allocation>& allocations,
                                               const RuleEngineFacade& ruleEngineFacade) const {
    for (Request* request : requests) {
        if (OneTimeRequest* oneTime = dynamic_cast<OneTimeRequest*>(request)) {
            processRequest(*oneTime, allocations, ruleEngineFacade);
        }
        else if (RecurringRequest* recurring = dynamic_cast<RecurringRequest*>(request)) {
            processRequest(*recurring, allocations, ruleEngineFacade);
        }
    }
}

bool GreedyAllocationStrategy::processRequest(OneTimeRequest& request,
                                              std::vector<Allocation>& allocations,
                                              const RuleEngineFacade& ruleEngineFacade) const {
    request.addHistoryEvent("Greedy strategy started evaluation");
    request.addHistoryEvent("evaluated");
    RuleEvaluationResult result = ruleEngineFacade.evaluateRequest(request, allocations);

    if (!result.isPassed()) {
        request.addHistoryEvent("Greedy strategy rejected request: " + result.getFailureReason());
        request.markRejected(result.getFailureReason());
        return false;
    }

    request.addHistoryEvent("Greedy strategy approved request");
    request.markApproved();

    int allocationId = nextAllocationId(allocations);
    Allocation allocation(allocationId, request.getId(),
                          request.getRequestedSpace(),
                          request.getRequestedTimeSlot());
    allocations.push_back(allocation);

    return true;
}

bool GreedyAllocationStrategy::processRequest(RecurringRequest& request,
                                              std::vector<Allocation>& allocations,
                                              const RuleEngineFacade& ruleEngineFacade) const {
    request.addHistoryEvent("Greedy strategy started evaluation");
    request.addHistoryEvent("evaluated");

    if (hasSelfConflict(request.getRequestedTimeSlots())) {
        request.addHistoryEvent("Greedy strategy rejected request: Self-conflicting recurring request");
        request.markRejected("Self-conflicting recurring request");
        return false;
    }

    RuleEvaluationResult result = ruleEngineFacade.evaluateRequest(request, allocations);

    if (!result.isPassed()) {
        request.addHistoryEvent("Greedy strategy rejected request: " + result.getFailureReason());
        request.markRejected(result.getFailureReason());
        return false;
    }

    request.addHistoryEvent("Greedy strategy approved request");
    request.markApproved();

    const std::vector<TimeSlot>& slots = request.getRequestedTimeSlots();
    for (const TimeSlot& slot : slots) {
        int allocationId = nextAllocationId(allocations);
        Allocation allocation(allocationId, request.getId(),
                              request.getRequestedSpace(), slot);
        allocations.push_back(allocation);
    }

    return true;
}
