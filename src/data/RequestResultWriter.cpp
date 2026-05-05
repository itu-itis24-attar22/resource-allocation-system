#include "RequestResultWriter.h"
#include <fstream>
#include <iostream>
#include "../models/OneTimeRequest.h"
#include "../models/RecurringRequest.h"
#include "../models/ExamRequest.h"
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
        if (const OneTimeRequest* oneTime = dynamic_cast<const OneTimeRequest*>(request)) {
            TimeSlot slot = oneTime->getRequestedTimeSlot();
            return dayToString(slot.getDay()) + " " +
                   std::to_string(slot.getStartHour()) + ":00-" +
                   std::to_string(slot.getEndHour()) + ":00";
        }

        if (const RecurringRequest* recurring = dynamic_cast<const RecurringRequest*>(request)) {
            std::string result;
            const std::vector<TimeSlot>& slots = recurring->getRequestedTimeSlots();

            for (size_t i = 0; i < slots.size(); i++) {
                result += dayToString(slots[i].getDay()) + " " +
                          std::to_string(slots[i].getStartHour()) + ":00-" +
                          std::to_string(slots[i].getEndHour()) + ":00";
                if (i + 1 < slots.size()) {
                    result += "; ";
                }
            }
            return result;
        }

        if (const ExamRequest* exam = dynamic_cast<const ExamRequest*>(request)) {
            TimeSlot slot = exam->getExamTimeSlot();
            return dayToString(slot.getDay()) + " " +
                   std::to_string(slot.getStartHour()) + ":00-" +
                   std::to_string(slot.getEndHour()) + ":00";
        }

        if (const InvalidRequest* invalid = dynamic_cast<const InvalidRequest*>(request)) {
            return invalid->getRawTimeInfo().empty() ? "None" : invalid->getRawTimeInfo();
        }

        return "";
    }

    std::string requestTypeToString(const Request* request) {
        if (dynamic_cast<const OneTimeRequest*>(request)) return "OneTime";
        if (dynamic_cast<const RecurringRequest*>(request)) return "Recurring";
        if (dynamic_cast<const ExamRequest*>(request)) return "Exam";
        if (const InvalidRequest* invalid = dynamic_cast<const InvalidRequest*>(request)) {
            return invalid->getRequestTypeLabel();
        }
        return "Unknown";
    }

    std::string examCourseCodeToString(const Request* request) {
        if (const ExamRequest* exam = dynamic_cast<const ExamRequest*>(request)) {
            return exam->getCourseCode();
        }
        return "None";
    }

    std::string examCourseNameToString(const Request* request) {
        if (const ExamRequest* exam = dynamic_cast<const ExamRequest*>(request)) {
            return exam->getCourseName();
        }
        return "None";
    }

    std::string examTypeToString(const Request* request) {
        if (const ExamRequest* exam = dynamic_cast<const ExamRequest*>(request)) {
            return exam->getExamType();
        }
        return "None";
    }

    std::string canSplitAcrossRoomsToString(const Request* request) {
        if (const ExamRequest* exam = dynamic_cast<const ExamRequest*>(request)) {
            return exam->getCanSplitAcrossRooms() ? "true" : "false";
        }
        return "None";
    }
}

void RequestResultWriter::writeRequestResults(const std::string& filename,
                                              const std::vector<Request*>& requests) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open request results file: " << filename << "\n";
        return;
    }

    file << "requestId,requestType,title,purpose,courseCode,courseName,examType,canSplitAcrossRooms,requesterName,requesterRole,priority,spaceName,spaceType,spaceBuilding,requiredBuilding,requiredFeature,participants,status,rejectionReason,timeInfo,lifecycleHistory\n";

    for (Request* request : requests) {
        request->addHistoryEvent("exported");

        file << request->getId() << ","
             << escapeCsv(requestTypeToString(request)) << ","
             << escapeCsv(request->getTitle()) << ","
             << escapeCsv(request->getPurpose()) << ","
             << escapeCsv(examCourseCodeToString(request)) << ","
             << escapeCsv(examCourseNameToString(request)) << ","
             << escapeCsv(examTypeToString(request)) << ","
             << escapeCsv(canSplitAcrossRoomsToString(request)) << ","
             << escapeCsv(request->getRequester()->getName()) << ","
             << escapeCsv(request->getRequester()->getRoleName()) << ","
             << request->getPriority() << ","
             << escapeCsv(request->getRequestedSpace()->getName()) << ","
             << escapeCsv(request->getRequestedSpace()->getType()) << ","
             << escapeCsv(request->getRequestedSpace()->getBuilding()) << ","
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
}
