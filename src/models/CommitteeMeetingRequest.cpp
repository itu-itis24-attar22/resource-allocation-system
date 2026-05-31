#include "CommitteeMeetingRequest.h"

namespace {
    std::string normalizeMeetingTopic(const std::string& title,
                                      const std::string& purpose) {
        if (!title.empty()) {
            return title;
        }

        if (!purpose.empty()) {
            return purpose;
        }

        return "Committee Meeting";
    }
}

CommitteeMeetingRequest::CommitteeMeetingRequest(
    int requestId,
    User* requester,
    Space* requestedSpace,
    const TimeSlot& preferredTimeSlot,
    int participantCount,
    const std::string& title,
    const std::string& purpose,
    const std::string& requiredFeature,
    const std::string& requiredBuilding
)
    : Request(requestId, requester, requestedSpace, participantCount,
              title, purpose, requiredFeature, requiredBuilding),
      meetingTopic(normalizeMeetingTopic(title, purpose)),
      durationMinutes(preferredTimeSlot.getEndMinutes() - preferredTimeSlot.getStartMinutes()),
      preferredTimeSlot(preferredTimeSlot) {}

std::string CommitteeMeetingRequest::getMeetingTopic() const {
    return meetingTopic;
}

int CommitteeMeetingRequest::getDurationMinutes() const {
    return durationMinutes;
}

TimeSlot CommitteeMeetingRequest::getPreferredTimeSlot() const {
    return preferredTimeSlot;
}

const std::vector<int>& CommitteeMeetingRequest::getRequiredParticipantIds() const {
    return requiredParticipantIds;
}

const std::vector<std::string>& CommitteeMeetingRequest::getParticipantRoles() const {
    return participantRoles;
}

void CommitteeMeetingRequest::addRequiredParticipant(
    int userId,
    const std::string& participantRole
) {
    requiredParticipantIds.push_back(userId);
    participantRoles.push_back(participantRole);
}

bool CommitteeMeetingRequest::isRecurring() const {
    return false;
}

std::string CommitteeMeetingRequest::getRequestType() const {
    return "CommitteeMeeting";
}
