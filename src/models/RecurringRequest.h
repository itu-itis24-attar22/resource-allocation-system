#ifndef RECURRINGREQUEST_H
#define RECURRINGREQUEST_H

#include <vector>
#include "Request.h"
#include "TimeSlot.h"

class RecurringRequest : public Request {
private:
    std::vector<TimeSlot> requestedTimeSlots;

public:
    RecurringRequest(int requestId, User* requester, Space* requestedSpace,
                     const std::vector<TimeSlot>& requestedTimeSlots,
                     int participantCount,
                     const std::string& requiredFeature = "",
                     const std::string& requiredBuilding = "");

    const std::vector<TimeSlot>& getRequestedTimeSlots() const;

    bool isRecurring() const override;
};

#endif
