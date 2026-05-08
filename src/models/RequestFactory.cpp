#include "RequestFactory.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include "Student.h"
#include "OneTimeRequest.h"
#include "RecurringRequest.h"
#include "ExamRequest.h"
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

    std::string trim(const std::string& text) {
        const auto first = std::find_if_not(text.begin(), text.end(), [](unsigned char ch) {
            return std::isspace(ch);
        });

        const auto last = std::find_if_not(text.rbegin(), text.rend(), [](unsigned char ch) {
            return std::isspace(ch);
        }).base();

        if (first >= last) {
            return "";
        }

        return std::string(first, last);
    }

    bool tryParseTimeOfDay(const std::string& text, int& minutesFromMidnight) {
        const std::string trimmed = trim(text);
        const size_t colonPosition = trimmed.find(':');

        if (colonPosition == std::string::npos) {
            int hour = 0;
            if (!tryParseInt(trimmed, hour)) {
                return false;
            }

            if (hour < 0 || hour > 24) {
                return false;
            }

            minutesFromMidnight = hour * 60;
            return true;
        }

        if (trimmed.find(':', colonPosition + 1) != std::string::npos) {
            return false;
        }

        const std::string hourText = trimmed.substr(0, colonPosition);
        const std::string minuteText = trimmed.substr(colonPosition + 1);
        int hour = 0;
        int minute = 0;

        if (!tryParseInt(hourText, hour) || !tryParseInt(minuteText, minute)) {
            return false;
        }

        if (hour < 0 || hour > 24 || minute < 0 || minute > 59) {
            return false;
        }

        if (hour == 24 && minute != 0) {
            return false;
        }

        minutesFromMidnight = hour * 60 + minute;
        return true;
    }

    bool tryParseSingleTimeSlot(const std::string& text,
                                TimeSlot& slot,
                                std::string& errorReason) {
        std::stringstream ss(text);
        std::string token;

        int day = 0;
        int startMinutes = 0;
        int endMinutes = 0;

        if (!std::getline(ss, token, '-') || !tryParseInt(trim(token), day)) {
            errorReason = "Malformed input";
            return false;
        }

        if (!std::getline(ss, token, '-') || !tryParseTimeOfDay(token, startMinutes)) {
            errorReason = "Malformed input";
            return false;
        }

        if (!std::getline(ss, token, '-') || !tryParseTimeOfDay(token, endMinutes)) {
            errorReason = "Malformed input";
            return false;
        }

        if (std::getline(ss, token, '-')) {
            errorReason = "Malformed input";
            return false;
        }

        try {
            slot = TimeSlot::fromMinutes(day, startMinutes, endMinutes);
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
                                             const std::string& title,
                                             const std::string& purpose,
                                             const std::string& requiredFeature,
                                             const std::string& requiredBuilding,
                                             const std::string& timeData,
                                             const std::string& rejectionReason,
                                             const std::string& courseCode,
                                             const std::string& courseName,
                                             const std::string& examType,
                                             bool canSplitAcrossRooms) {
    (void)courseCode;
    (void)courseName;
    (void)examType;
    (void)canSplitAcrossRooms;

    InvalidRequest* request = new InvalidRequest(
        requestId,
        requester ? requester : placeholderUser(),
        requestedSpace ? requestedSpace : placeholderSpace(),
        participantCount,
        requestType.empty() ? "Unknown" : requestType,
        timeData,
        title,
        purpose,
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
                                       const std::string& title,
                                       const std::string& purpose,
                                       const std::string& requiredFeature,
                                       const std::string& requiredBuilding,
                                       const std::string& timeData,
                                       const std::string& courseCode,
                                       const std::string& courseName,
                                       const std::string& examType,
                                       bool canSplitAcrossRooms) {
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
                title,
                purpose,
                requiredFeature,
                requiredBuilding,
                timeData,
                errorReason,
                courseCode,
                courseName,
                examType,
                canSplitAcrossRooms
            );
        }

        return new OneTimeRequest(
            requestId,
            requester,
            requestedSpace,
            slot,
            participantCount,
            title,
            purpose,
            requiredFeature,
            requiredBuilding
        );
    }

    if (requestType == "Exam") {
        TimeSlot slot(1, 0, 1);
        std::string errorReason;
        if (!tryParseSingleTimeSlot(timeData, slot, errorReason)) {
            return createInvalidRequest(
                requestId,
                requestType,
                requester,
                requestedSpace,
                participantCount,
                title,
                purpose,
                requiredFeature,
                requiredBuilding,
                timeData,
                errorReason,
                courseCode,
                courseName,
                examType,
                canSplitAcrossRooms
            );
        }

        return new ExamRequest(
            requestId,
            requester,
            requestedSpace,
            slot,
            participantCount,
            title,
            purpose,
            requiredFeature,
            requiredBuilding,
            courseCode,
            courseName,
            examType,
            canSplitAcrossRooms
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
                title,
                purpose,
                requiredFeature,
                requiredBuilding,
                timeData,
                errorReason,
                courseCode,
                courseName,
                examType,
                canSplitAcrossRooms
            );
        }

        return new RecurringRequest(
            requestId,
            requester,
            requestedSpace,
            slots,
            participantCount,
            title,
            purpose,
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
        title,
        purpose,
        requiredFeature,
        requiredBuilding,
        timeData,
        "Malformed input",
        courseCode,
        courseName,
        examType,
        canSplitAcrossRooms
    );
}
