#include "MeetingTimeSuggestionService.h"

namespace {
    constexpr int Monday = 1;
    constexpr int Friday = 5;
    constexpr int WorkingDayStartMinutes = 9 * 60;
    constexpr int WorkingDayEndMinutes = 17 * 60;
    constexpr int ScanStepMinutes = 30;
}

bool MeetingTimeSuggestionService::areParticipantsAvailable(
    const CommitteeMeetingRequest& request,
    const TimeSlot& candidateSlot,
    const std::vector<UserBusySlot>& userBusySlots
) const {
    const std::vector<int>& participantIds = request.getRequiredParticipantIds();

    if (participantIds.empty()) {
        return false;
    }

    for (int participantId : participantIds) {
        if (!userAvailabilityService.isUserAvailable(participantId,
                                                     candidateSlot,
                                                     userBusySlots)) {
            return false;
        }
    }

    return true;
}

bool MeetingTimeSuggestionService::isRequestedSpaceAvailable(
    const Space* requestedSpace,
    const TimeSlot& candidateSlot,
    const std::vector<Allocation>& allocations
) const {
    if (!requestedSpace) {
        return false;
    }

    for (const Allocation& allocation : allocations) {
        const Space* allocatedSpace = allocation.getSpace();
        if (allocatedSpace &&
            allocatedSpace->getId() == requestedSpace->getId() &&
            allocation.getTimeSlot().overlapsWith(candidateSlot)) {
            return false;
        }
    }

    return true;
}

std::vector<MeetingTimeSuggestion> MeetingTimeSuggestionService::suggestTimes(
    const CommitteeMeetingRequest& request,
    Space* requestedSpace,
    const std::vector<UserBusySlot>& userBusySlots,
    const std::vector<Allocation>& allocations,
    int maxSuggestions
) const {
    std::vector<MeetingTimeSuggestion> suggestions;

    if (!requestedSpace || maxSuggestions <= 0) {
        return suggestions;
    }

    const int durationMinutes = request.getDurationMinutes();
    if (durationMinutes <= 0 ||
        durationMinutes > WorkingDayEndMinutes - WorkingDayStartMinutes) {
        return suggestions;
    }

    for (int day = Monday; day <= Friday; ++day) {
        for (int startMinutes = WorkingDayStartMinutes;
             startMinutes + durationMinutes <= WorkingDayEndMinutes;
             startMinutes += ScanStepMinutes) {
            const int endMinutes = startMinutes + durationMinutes;
            TimeSlot candidateSlot =
                TimeSlot::fromMinutes(day, startMinutes, endMinutes);

            if (!areParticipantsAvailable(request, candidateSlot, userBusySlots)) {
                continue;
            }

            if (!isRequestedSpaceAvailable(requestedSpace, candidateSlot, allocations)) {
                continue;
            }

            suggestions.emplace_back(
                candidateSlot,
                requestedSpace->getId(),
                "All required participants and requested room are available"
            );

            if (static_cast<int>(suggestions.size()) >= maxSuggestions) {
                return suggestions;
            }
        }
    }

    return suggestions;
}
