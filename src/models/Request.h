#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <vector>
#include "User.h"
#include "Space.h"
#include "Status.h"

class Request {
protected:
    int requestId;
    User* requester;
    Space* requestedSpace;
    int participantCount;
    int priority;
    RequestStatus status;
    std::string title;
    std::string purpose;
    std::string requiredFeature;
    std::string requiredBuilding;
    std::string rejectionReason;
    std::vector<std::string> lifecycleHistory;

public:
    Request(int requestId, User* requester, Space* requestedSpace,
            int participantCount,
            const std::string& title = "",
            const std::string& purpose = "",
            const std::string& requiredFeature = "",
            const std::string& requiredBuilding = "");

    virtual ~Request() = default;

    int getId() const;
    const User* getRequester() const;
    Space* getRequestedSpace() const;
    int getParticipantCount() const;
    int getPriority() const;
    RequestStatus getStatus() const;
    std::string getTitle() const;
    std::string getPurpose() const;
    std::string getRequiredFeature() const;
    std::string getRequiredBuilding() const;
    std::string getRejectionReason() const;
    const std::vector<std::string>& getLifecycleHistory() const;

    void addHistoryEvent(const std::string& event);

    virtual void markApproved();
    virtual void markRejected(const std::string& reason);

    virtual bool isRecurring() const = 0;
    virtual std::string getRequestType() const = 0;
};

#endif
