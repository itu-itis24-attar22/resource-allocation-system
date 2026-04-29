#include "RequestFactory.h"
#include <sstream>
#include "Student.h"
#include "OneTimeRequest.h"
#include "RecurringRequest.h"
#include "InvalidRequest.h"
#include "TimeSlot.h"

namespace {
    class PlaceholderSpace : public Space {
    public:
        PlaceholderSpace()
            : Space(-1, "UnknownSpace", 0, false, false, false, false, "Unknown") {}

        std::string getType() const override {
            return "Unknown";
        }
    };

    User* placeholderUser() {
        static Student user(-1, "UnknownUser");
        return &user;
    }

    Space* placeholderSpace() {
        static PlaceholderSpace space;
        return &space;
    }

    bool tryParseInt(const std::string& text, int& value) {
        try {
            size_t parsedLength = 0;
            value = std::stoi(text, &parsedLength);
            return parsedLength == text.size();
        } catch (...) {
            return false;
        }
    }

    bool tryParseSingleTimeSlot(const std::string& text,
                                TimeSlot& slot,
                                std::string& errorReason) {
        std::stringstream ss(text);
        std::string token;

        int day = 0;
        int startHour = 0;
        int endHour = 0;

        if (!std::getline(ss, token, '-') || !tryParseInt(token, day)) {
            errorReason = "Malformed input";
            return false;
        }

        if (!std::getline(ss, token, '-') || !tryParseInt(token, startHour)) {
            errorReason = "Malformed input";
            return false;
        }

        if (!std::getline(ss, token, '-') || !tryParseInt(token, endHour)) {
            errorReason = "Malformed input";
            return false;
        }

        try {
            slot = TimeSlot(day, startHour, endHour);
            return true;
        } catch (...) {
            errorReason = "Invalid time slot";
            return false;
        }
    }

    bool tryParseRecurringTimeSlots(const std::string& text,
                                    std::vector<TimeSlot>& slots,
                                    std::string& errorReason) {
        std::stringstream ss(text);
        std::string part;

        while (std::getline(ss, part, ';')) {
            if (!part.empty()) {
                TimeSlot slot(1, 0, 1);
                if (!tryParseSingleTimeSlot(part, slot, errorReason)) {
                    return false;
                }
                slots.push_back(slot);
            }
        }

        if (slots.empty()) {
            errorReason = "Malformed input";
            return false;
        }

        return true;
    }
}

Request* RequestFactory::createInvalidRequest(int requestId,
                                             const std::string& requestType,
                                             User* requester,
                                             Space* requestedSpace,
                                             int participantCount,
                                             const std::string& requiredFeature,
                                             const std::string& requiredBuilding,
                                             const std::string& timeData,
                                             const std::string& rejectionReason) {
    InvalidRequest* request = new InvalidRequest(
        requestId,
        requester ? requester : placeholderUser(),
        requestedSpace ? requestedSpace : placeholderSpace(),
        participantCount,
        requestType.empty() ? "Unknown" : requestType,
        timeData,
        requiredFeature,
        requiredBuilding
    );

    request->addHistoryEvent("evaluated");
    request->markRejected(rejectionReason);
    return request;
}

Request* RequestFactory::createRequest(int requestId,
                                       const std::string& requestType,
                                       User* requester,
                                       Space* requestedSpace,
                                       int participantCount,
                                       const std::string& requiredFeature,
                                       const std::string& requiredBuilding,
                                       const std::string& timeData) {
    if (requestType == "OneTime") {
        TimeSlot slot(1, 0, 1);
        std::string errorReason;
        if (!tryParseSingleTimeSlot(timeData, slot, errorReason)) {
            return createInvalidRequest(
                requestId,
                requestType,
                requester,
                requestedSpace,
                participantCount,
                requiredFeature,
                requiredBuilding,
                timeData,
                errorReason
            );
        }

        return new OneTimeRequest(
            requestId,
            requester,
            requestedSpace,
            slot,
            participantCount,
            requiredFeature,
            requiredBuilding
        );
    }

    if (requestType == "Recurring") {
        std::vector<TimeSlot> slots;
        std::string errorReason;
        if (!tryParseRecurringTimeSlots(timeData, slots, errorReason)) {
            return createInvalidRequest(
                requestId,
                requestType,
                requester,
                requestedSpace,
                participantCount,
                requiredFeature,
                requiredBuilding,
                timeData,
                errorReason
            );
        }

        return new RecurringRequest(
            requestId,
            requester,
            requestedSpace,
            slots,
            participantCount,
            requiredFeature,
            requiredBuilding
        );
    }

    return createInvalidRequest(
        requestId,
        requestType,
        requester,
        requestedSpace,
        participantCount,
        requiredFeature,
        requiredBuilding,
        timeData,
        "Malformed input"
    );
}
