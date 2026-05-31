#ifndef COMMITTEEMEETINGREQUEST_H
#define COMMITTEEMEETINGREQUEST_H

#include <string>
#include <vector>
#include "Request.h"
#include "TimeSlot.h"

class CommitteeMeetingRequest : public Request {
private:
    std::string meetingTopic;
    int durationMinutes;
    TimeSlot preferredTimeSlot;
    std::vector<int> requiredParticipantIds;
    std::vector<std::string> participantRoles;

public:
    CommitteeMeetingRequest(int requestId,
                            User* requester,
                            Space* requestedSpace,
                            const TimeSlot& preferredTimeSlot,
                            int participantCount,
                            const std::string& title = "",
                            const std::string& purpose = "",
                            const std::string& requiredFeature = "",
                            const std::string& requiredBuilding = "");

    std::string getMeetingTopic() const;
    int getDurationMinutes() const;
    TimeSlot getPreferredTimeSlot() const;
    const std::vector<int>& getRequiredParticipantIds() const;
    const std::vector<std::string>& getParticipantRoles() const;
    void addRequiredParticipant(int userId, const std::string& participantRole);

    bool isRecurring() const override;
    std::string getRequestType() const override;
};

#endif
