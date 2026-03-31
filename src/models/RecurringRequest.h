#ifndef RECURRINGREQUEST_H
#define RECURRINGREQUEST_H

#include <vector>
#include <string>
#include "User.h"
#include "Space.h"
#include "TimeSlot.h"
#include "Status.h"

class RecurringRequest {
private:
    int requestId;
    User requester;
    Space* requestedSpace;
    std::vector<TimeSlot> requestedTimeSlots;
    int participantCount;
    RequestStatus status;
    std::string requiredFeature;
    std::string requiredBuilding;
    std::string rejectionReason;

public:
    RecurringRequest(int requestId, const User& requester, Space* requestedSpace,
                     const std::vector<TimeSlot>& requestedTimeSlots,
                     int participantCount,
                     const std::string& requiredFeature = "",
                     const std::string& requiredBuilding = "");

    int getId() const;
    User getRequester() const;
    Space* getRequestedSpace() const;
    const std::vector<TimeSlot>& getRequestedTimeSlots() const;
    int getParticipantCount() const;
    RequestStatus getStatus() const;
    std::string getRequiredFeature() const;
    std::string getRequiredBuilding() const;
    std::string getRejectionReason() const;

    void markApproved();
    void markRejected(const std::string& reason);
};

#endif