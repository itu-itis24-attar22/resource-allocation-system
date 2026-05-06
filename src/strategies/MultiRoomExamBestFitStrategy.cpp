#include "MultiRoomExamBestFitStrategy.h"
#include <algorithm>
#include <limits>
#include <utility>
#include "../models/InvalidRequest.h"

namespace {
    struct CandidateRoom {
        const Space* space;
        int capacity;
    };

    struct BestFitState {
        bool reachable = false;
        int roomCount = std::numeric_limits<int>::max();
        int previousCapacity = -1;
        int candidateIndex = -1;
    };

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
        request.addHistoryEvent("Multi-room exam best-fit rejected request: " + reason);
        request.markRejected(reason);
    }

    std::vector<CandidateRoom> findCandidateRooms(const ExamRequest& request,
                                                  const std::vector<Space*>& spaces,
                                                  const std::vector<Allocation>& allocations) {
        std::vector<CandidateRoom> candidates;
        const User* requester = request.getRequester();
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

            if (!requester || !requester->canRequestSpaceType(space->getType())) {
                continue;
            }

            if (space->getCapacity() <= 0) {
                continue;
            }

            candidates.push_back({space, space->getCapacity()});
        }

        return candidates;
    }

    std::vector<const Space*> selectBestFitRooms(const std::vector<CandidateRoom>& candidates,
                                                 int participantCount) {
        int totalCapacity = 0;
        for (const CandidateRoom& candidate : candidates) {
            totalCapacity += candidate.capacity;
        }

        if (participantCount <= 0 || totalCapacity < participantCount) {
            return {};
        }

        std::vector<BestFitState> dp(totalCapacity + 1);
        dp[0].reachable = true;
        dp[0].roomCount = 0;

        for (size_t index = 0; index < candidates.size(); ++index) {
            const int roomCapacity = candidates[index].capacity;

            for (int capacity = totalCapacity - roomCapacity; capacity >= 0; --capacity) {
                if (!dp[capacity].reachable) {
                    continue;
                }

                const int nextCapacity = capacity + roomCapacity;
                const int nextRoomCount = dp[capacity].roomCount + 1;

                if (!dp[nextCapacity].reachable ||
                    nextRoomCount < dp[nextCapacity].roomCount) {
                    dp[nextCapacity].reachable = true;
                    dp[nextCapacity].roomCount = nextRoomCount;
                    dp[nextCapacity].previousCapacity = capacity;
                    dp[nextCapacity].candidateIndex = static_cast<int>(index);
                }
            }
        }

        int bestCapacity = -1;
        for (int capacity = participantCount; capacity <= totalCapacity; ++capacity) {
            if (dp[capacity].reachable) {
                bestCapacity = capacity;
                break;
            }
        }

        if (bestCapacity < 0) {
            return {};
        }

        std::vector<const Space*> selectedRooms;
        int currentCapacity = bestCapacity;
        while (currentCapacity > 0) {
            const BestFitState& state = dp[currentCapacity];
            if (state.candidateIndex < 0) {
                break;
            }

            selectedRooms.push_back(candidates[state.candidateIndex].space);
            currentCapacity = state.previousCapacity;
        }

        std::reverse(selectedRooms.begin(), selectedRooms.end());
        return selectedRooms;
    }
}

void MultiRoomExamBestFitStrategy::processRequests(const std::vector<Request*>& requests,
                                                   const std::vector<Space*>& spaces,
                                                   std::vector<Allocation>& allocations,
                                                   const RuleEngineFacade& ruleEngineFacade) const {
    for (Request* request : requests) {
        if (OneTimeRequest* oneTime = dynamic_cast<OneTimeRequest*>(request)) {
            oneTime->addHistoryEvent("Multi-room exam best-fit batch processing started");
            processRequest(*oneTime, allocations, ruleEngineFacade);
        }
        else if (RecurringRequest* recurring = dynamic_cast<RecurringRequest*>(request)) {
            recurring->addHistoryEvent("Multi-room exam best-fit batch processing started");
            processRequest(*recurring, allocations, ruleEngineFacade);
        }
        else if (ExamRequest* exam = dynamic_cast<ExamRequest*>(request)) {
            exam->addHistoryEvent("Multi-room exam best-fit batch processing started");

            if (exam->getCanSplitAcrossRooms()) {
                processSplittableExamRequest(*exam, spaces, allocations);
            } else {
                processRequest(*exam, allocations, ruleEngineFacade);
            }
        }
        else if (InvalidRequest* invalid = dynamic_cast<InvalidRequest*>(request)) {
            invalid->addHistoryEvent("Multi-room exam best-fit skipped invalid request");
        }
    }
}

bool MultiRoomExamBestFitStrategy::processRequest(OneTimeRequest& request,
                                                  std::vector<Allocation>& allocations,
                                                  const RuleEngineFacade& ruleEngineFacade) const {
    return greedyStrategy.processRequest(request, allocations, ruleEngineFacade);
}

bool MultiRoomExamBestFitStrategy::processRequest(RecurringRequest& request,
                                                  std::vector<Allocation>& allocations,
                                                  const RuleEngineFacade& ruleEngineFacade) const {
    return greedyStrategy.processRequest(request, allocations, ruleEngineFacade);
}

bool MultiRoomExamBestFitStrategy::processRequest(ExamRequest& request,
                                                  std::vector<Allocation>& allocations,
                                                  const RuleEngineFacade& ruleEngineFacade) const {
    return greedyStrategy.processRequest(request, allocations, ruleEngineFacade);
}

bool MultiRoomExamBestFitStrategy::processSplittableExamRequest(ExamRequest& request,
                                                                const std::vector<Space*>& spaces,
                                                                std::vector<Allocation>& allocations) const {
    request.addHistoryEvent("Multi-room exam best-fit started evaluation");
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

    const std::vector<CandidateRoom> candidates = findCandidateRooms(request, spaces, allocations);
    const std::vector<const Space*> selectedRooms =
        selectBestFitRooms(candidates, request.getParticipantCount());

    if (selectedRooms.empty()) {
        rejectExam(request, "Not enough available classroom capacity for exam");
        return false;
    }

    int selectedCapacity = 0;
    for (const Space* room : selectedRooms) {
        selectedCapacity += room->getCapacity();
    }

    request.addHistoryEvent("Multi-room exam best-fit approved request across "
                            + std::to_string(selectedRooms.size()) + " room(s) with "
                            + std::to_string(selectedCapacity - request.getParticipantCount())
                            + " unused seat(s)");
    request.markApproved();

    int assignedTotal = 0;
    const TimeSlot examTimeSlot = request.getExamTimeSlot();
    for (const Space* room : selectedRooms) {
        const int remainingParticipants = request.getParticipantCount() - assignedTotal;
        const int assignedParticipants = std::min(room->getCapacity(), remainingParticipants);

        Allocation allocation(nextAllocationId(allocations),
                              request.getId(),
                              room,
                              examTimeSlot,
                              assignedParticipants);
        allocations.push_back(allocation);
        assignedTotal += assignedParticipants;
    }

    return true;
}
