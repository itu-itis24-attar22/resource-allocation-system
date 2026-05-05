#include "MultiRoomExamGreedyStrategy.h"
#include <algorithm>
#include <utility>
#include "../models/InvalidRequest.h"

namespace {
    int nextAllocationId(const std::vector<Allocation>& allocations) {
        return static_cast<int>(allocations.size()) + 1;
    }

    bool isSpaceAvailableAt(const Space* space,
                            const TimeSlot& timeSlot,
                            const std::vector<Allocation>& allocations) {
        for (const Allocation& allocation : allocations) {
            const bool sameSpace = allocation.getSpace()->getId() == space->getId();
            const bool overlaps = allocation.getTimeSlot().overlapsWith(timeSlot);

            if (sameSpace && overlaps) {
                return false;
            }
        }

        return true;
    }

    bool matchesRequiredBuilding(const Space* space, const std::string& requiredBuilding) {
        return requiredBuilding.empty() || space->getBuilding() == requiredBuilding;
    }

    void rejectExam(ExamRequest& request, const std::string& reason) {
        request.addHistoryEvent("Multi-room exam greedy rejected request: " + reason);
        request.markRejected(reason);
    }
}

void MultiRoomExamGreedyStrategy::processRequests(const std::vector<Request*>& requests,
                                                  const std::vector<Space*>& spaces,
                                                  std::vector<Allocation>& allocations,
                                                  const RuleEngineFacade& ruleEngineFacade) const {
    for (Request* request : requests) {
        if (OneTimeRequest* oneTime = dynamic_cast<OneTimeRequest*>(request)) {
            oneTime->addHistoryEvent("Multi-room exam greedy batch processing started");
            processRequest(*oneTime, allocations, ruleEngineFacade);
        }
        else if (RecurringRequest* recurring = dynamic_cast<RecurringRequest*>(request)) {
            recurring->addHistoryEvent("Multi-room exam greedy batch processing started");
            processRequest(*recurring, allocations, ruleEngineFacade);
        }
        else if (ExamRequest* exam = dynamic_cast<ExamRequest*>(request)) {
            exam->addHistoryEvent("Multi-room exam greedy batch processing started");

            if (exam->getCanSplitAcrossRooms()) {
                processSplittableExamRequest(*exam, spaces, allocations);
            } else {
                processRequest(*exam, allocations, ruleEngineFacade);
            }
        }
        else if (InvalidRequest* invalid = dynamic_cast<InvalidRequest*>(request)) {
            invalid->addHistoryEvent("Multi-room exam greedy skipped invalid request");
        }
    }
}

bool MultiRoomExamGreedyStrategy::processRequest(OneTimeRequest& request,
                                                 std::vector<Allocation>& allocations,
                                                 const RuleEngineFacade& ruleEngineFacade) const {
    return greedyStrategy.processRequest(request, allocations, ruleEngineFacade);
}

bool MultiRoomExamGreedyStrategy::processRequest(RecurringRequest& request,
                                                 std::vector<Allocation>& allocations,
                                                 const RuleEngineFacade& ruleEngineFacade) const {
    return greedyStrategy.processRequest(request, allocations, ruleEngineFacade);
}

bool MultiRoomExamGreedyStrategy::processRequest(ExamRequest& request,
                                                 std::vector<Allocation>& allocations,
                                                 const RuleEngineFacade& ruleEngineFacade) const {
    return greedyStrategy.processRequest(request, allocations, ruleEngineFacade);
}

bool MultiRoomExamGreedyStrategy::processSplittableExamRequest(ExamRequest& request,
                                                               const std::vector<Space*>& spaces,
                                                               std::vector<Allocation>& allocations) const {
    request.addHistoryEvent("Multi-room exam greedy started evaluation");
    request.addHistoryEvent("evaluated");

    const User* requester = request.getRequester();
    if (!requester || !requester->canSubmitRequestType(request.getRequestType())) {
        rejectExam(request, "User is not allowed to submit Exam requests");
        return false;
    }

    if (!requester->canRequestSpaceType("Classroom")) {
        rejectExam(request, "User role not authorized for requested space type");
        return false;
    }

    std::vector<std::pair<const Space*, int>> selectedRooms;
    int assignedTotal = 0;
    const int requestedParticipants = request.getParticipantCount();
    const TimeSlot examTimeSlot = request.getExamTimeSlot();

    for (const Space* space : spaces) {
        if (!space || space->getType() != "Classroom") {
            continue;
        }

        if (!space->getIsAvailable()) {
            continue;
        }

        if (!matchesRequiredBuilding(space, request.getRequiredBuilding())) {
            continue;
        }

        if (!space->hasFeature(request.getRequiredFeature())) {
            continue;
        }

        if (!isSpaceAvailableAt(space, examTimeSlot, allocations)) {
            continue;
        }

        if (!requester->canRequestSpaceType(space->getType())) {
            continue;
        }

        const int remainingParticipants = requestedParticipants - assignedTotal;
        const int assignedParticipants = std::min(space->getCapacity(), remainingParticipants);

        if (assignedParticipants <= 0) {
            continue;
        }

        selectedRooms.push_back(std::make_pair(space, assignedParticipants));
        assignedTotal += assignedParticipants;

        if (assignedTotal >= requestedParticipants) {
            break;
        }
    }

    if (assignedTotal < requestedParticipants) {
        rejectExam(request, "Not enough available classroom capacity for exam");
        return false;
    }

    request.addHistoryEvent("Multi-room exam greedy approved request across "
                            + std::to_string(selectedRooms.size()) + " room(s)");
    request.markApproved();

    for (const auto& roomAssignment : selectedRooms) {
        Allocation allocation(nextAllocationId(allocations),
                              request.getId(),
                              roomAssignment.first,
                              examTimeSlot,
                              roomAssignment.second);
        allocations.push_back(allocation);
    }

    return true;
}
