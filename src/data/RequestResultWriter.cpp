#include "RequestResultWriter.h"
#include <fstream>
#include <iostream>
#include "../models/OneTimeRequest.h"
#include "../models/RecurringRequest.h"
#include "../models/ExamRequest.h"
#include "../models/CommitteeMeetingRequest.h"
#include "../models/InvalidRequest.h"

namespace {
    std::string escapeCsv(const std::string& value) {
        bool needsQuotes = false;
        std::string escaped;

        for (char ch : value) {
            if (ch == '"' || ch == ',' || ch == '\n' || ch == '\r') {
                needsQuotes = true;
            }

            if (ch == '"') {
                escaped += "\"\"";
            } else {
                escaped += ch;
            }
        }

        if (!needsQuotes) {
            return escaped;
        }

        return "\"" + escaped + "\"";
    }

    std::string buildHistoryInfo(const Request* request) {
        if (!request) {
            return "";
        }

        std::string result;
        const std::vector<std::string>& history = request->getLifecycleHistory();

        for (size_t i = 0; i < history.size(); i++) {
            result += history[i];
            if (i + 1 < history.size()) {
                result += " | ";
            }
        }

        return result;
    }

    std::string dayToString(int day) {
        switch (day) {
            case 1: return "Monday";
            case 2: return "Tuesday";
            case 3: return "Wednesday";
            case 4: return "Thursday";
            case 5: return "Friday";
            case 6: return "Saturday";
            case 7: return "Sunday";
            default: return "Unknown";
        }
    }

    std::string requestStatusToString(RequestStatus status) {
        switch (status) {
            case RequestStatus::Pending: return "Pending";
            case RequestStatus::Approved: return "Approved";
            case RequestStatus::Rejected: return "Rejected";
            default: return "Unknown";
        }
    }

    std::string buildTimeInfo(const Request* request) {
        if (!request) {
            return "";
        }

        if (const OneTimeRequest* oneTime = dynamic_cast<const OneTimeRequest*>(request)) {
            TimeSlot slot = oneTime->getRequestedTimeSlot();
            return dayToString(slot.getDay()) + " " +
                   slot.getStartTimeString() + "-" +
                   slot.getEndTimeString();
        }

        if (const RecurringRequest* recurring = dynamic_cast<const RecurringRequest*>(request)) {
            std::string result;
            const std::vector<TimeSlot>& slots = recurring->getRequestedTimeSlots();

            for (size_t i = 0; i < slots.size(); i++) {
                result += dayToString(slots[i].getDay()) + " " +
                          slots[i].getStartTimeString() + "-" +
                          slots[i].getEndTimeString();
                if (i + 1 < slots.size()) {
                    result += "; ";
                }
            }
            return result;
        }

        if (const ExamRequest* exam = dynamic_cast<const ExamRequest*>(request)) {
            TimeSlot slot = exam->getExamTimeSlot();
            return dayToString(slot.getDay()) + " " +
                   slot.getStartTimeString() + "-" +
                   slot.getEndTimeString();
        }

        if (const CommitteeMeetingRequest* committee =
                dynamic_cast<const CommitteeMeetingRequest*>(request)) {
            TimeSlot slot = committee->getPreferredTimeSlot();
            return dayToString(slot.getDay()) + " " +
                   slot.getStartTimeString() + "-" +
                   slot.getEndTimeString();
        }

        if (const InvalidRequest* invalid = dynamic_cast<const InvalidRequest*>(request)) {
            return invalid->getRawTimeInfo().empty() ? "None" : invalid->getRawTimeInfo();
        }

        return "";
    }

    std::string requestTypeToString(const Request* request) {
        if (!request) {
            return "Unknown";
        }

        if (dynamic_cast<const OneTimeRequest*>(request)) return "OneTime";
        if (dynamic_cast<const RecurringRequest*>(request)) return "Recurring";
        if (dynamic_cast<const ExamRequest*>(request)) return "Exam";
        if (dynamic_cast<const CommitteeMeetingRequest*>(request)) return "CommitteeMeeting";
        if (const InvalidRequest* invalid = dynamic_cast<const InvalidRequest*>(request)) {
            return invalid->getRequestTypeLabel();
        }
        return "Unknown";
    }

    std::string examCourseCodeToString(const Request* request) {
        if (!request) {
            return "None";
        }

        if (const ExamRequest* exam = dynamic_cast<const ExamRequest*>(request)) {
            return exam->getCourseCode();
        }
        return "None";
    }

    std::string examCourseNameToString(const Request* request) {
        if (!request) {
            return "None";
        }

        if (const ExamRequest* exam = dynamic_cast<const ExamRequest*>(request)) {
            return exam->getCourseName();
        }
        return "None";
    }

    std::string examTypeToString(const Request* request) {
        if (!request) {
            return "None";
        }

        if (const ExamRequest* exam = dynamic_cast<const ExamRequest*>(request)) {
            return exam->getExamType();
        }
        return "None";
    }

    std::string canSplitAcrossRoomsToString(const Request* request) {
        if (!request) {
            return "None";
        }

        if (const ExamRequest* exam = dynamic_cast<const ExamRequest*>(request)) {
            return exam->getCanSplitAcrossRooms() ? "true" : "false";
        }
        return "None";
    }

    std::string committeeParticipantsToString(const Request* request) {
        if (!request) {
            return "None";
        }

        const CommitteeMeetingRequest* committee =
            dynamic_cast<const CommitteeMeetingRequest*>(request);

        if (!committee) {
            return "None";
        }

        const std::vector<int>& participantIds = committee->getRequiredParticipantIds();
        const std::vector<std::string>& participantRoles = committee->getParticipantRoles();

        if (participantIds.empty()) {
            return "None";
        }

        std::string result;
        for (size_t i = 0; i < participantIds.size(); i++) {
            result += std::to_string(participantIds[i]);
            result += ":";
            result += i < participantRoles.size() ? participantRoles[i] : "Participant";

            if (i + 1 < participantIds.size()) {
                result += " | ";
            }
        }

        return result;
    }

    std::string requesterNameToString(const Request* request) {
        return request && request->getRequester() ? request->getRequester()->getName() : "";
    }

    std::string requesterRoleToString(const Request* request) {
        return request && request->getRequester() ? request->getRequester()->getRoleName() : "";
    }

    std::string spaceNameToString(const Request* request) {
        return request && request->getRequestedSpace() ? request->getRequestedSpace()->getName() : "";
    }

    std::string spaceTypeToString(const Request* request) {
        return request && request->getRequestedSpace() ? request->getRequestedSpace()->getType() : "";
    }

    std::string spaceBuildingToString(const Request* request) {
        return request && request->getRequestedSpace() ? request->getRequestedSpace()->getBuilding() : "";
    }
}

bool RequestResultWriter::writeRequestResults(const std::string& filename,
                                              const std::vector<Request*>& requests) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open request results file: " << filename << "\n";
        return false;
    }

    file << "requestId,requestType,title,purpose,courseCode,courseName,examType,canSplitAcrossRooms,committeeParticipants,requesterName,requesterRole,priority,spaceName,spaceType,spaceBuilding,requiredBuilding,requiredFeature,participants,status,rejectionReason,timeInfo,lifecycleHistory\n";

    for (Request* request : requests) {
        if (!request) {
            std::cerr << "Warning: Skipping null request while writing request results.\n";
            continue;
        }

        request->addHistoryEvent("exported");

        file << request->getId() << ","
             << escapeCsv(requestTypeToString(request)) << ","
             << escapeCsv(request->getTitle()) << ","
             << escapeCsv(request->getPurpose()) << ","
             << escapeCsv(examCourseCodeToString(request)) << ","
             << escapeCsv(examCourseNameToString(request)) << ","
             << escapeCsv(examTypeToString(request)) << ","
             << escapeCsv(canSplitAcrossRoomsToString(request)) << ","
             << escapeCsv(committeeParticipantsToString(request)) << ","
             << escapeCsv(requesterNameToString(request)) << ","
             << escapeCsv(requesterRoleToString(request)) << ","
             << request->getPriority() << ","
             << escapeCsv(spaceNameToString(request)) << ","
             << escapeCsv(spaceTypeToString(request)) << ","
             << escapeCsv(spaceBuildingToString(request)) << ","
             << escapeCsv(request->getRequiredBuilding().empty() ? "None" : request->getRequiredBuilding()) << ","
             << escapeCsv(request->getRequiredFeature().empty() ? "None" : request->getRequiredFeature()) << ","
             << request->getParticipantCount() << ","
             << escapeCsv(requestStatusToString(request->getStatus())) << ","
             << escapeCsv(request->getRejectionReason().empty() ? "None" : request->getRejectionReason()) << ","
             << escapeCsv(buildTimeInfo(request)) << ","
             << escapeCsv(buildHistoryInfo(request))
             << "\n";
    }

    file.close();
    if (!file) {
        std::cerr << "Error: Failed while writing request results file: "
                  << filename << "\n";
        return false;
    }

    return true;
}
