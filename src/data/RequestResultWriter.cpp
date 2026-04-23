#include "RequestResultWriter.h"
#include <fstream>
#include <iostream>
#include "../models/OneTimeRequest.h"
#include "../models/RecurringRequest.h"
#include "../models/UserRole.h"

namespace {
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

        return "";
    }

    std::string requestTypeToString(const Request* request) {
        if (dynamic_cast<const OneTimeRequest*>(request)) return "OneTime";
        if (dynamic_cast<const RecurringRequest*>(request)) return "Recurring";
        return "Unknown";
    }
}

void RequestResultWriter::writeRequestResults(const std::string& filename,
                                              const std::vector<Request*>& requests) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open request results file: " << filename << "\n";
        return;
    }

    file << "requestId,requestType,requesterName,requesterRole,priority,spaceName,spaceType,spaceBuilding,requiredBuilding,requiredFeature,participants,status,rejectionReason,timeInfo,lifecycleHistory\n";

    for (Request* request : requests) {
        request->addHistoryEvent("exported");

        file << request->getId() << ","
             << requestTypeToString(request) << ","
             << request->getRequester().getName() << ","
             << userRoleToString(request->getRequester().getRole()) << ","
             << request->getPriority() << ","
             << request->getRequestedSpace()->getName() << ","
             << request->getRequestedSpace()->getType() << ","
             << request->getRequestedSpace()->getBuilding() << ","
             << (request->getRequiredBuilding().empty() ? "None" : request->getRequiredBuilding()) << ","
             << (request->getRequiredFeature().empty() ? "None" : request->getRequiredFeature()) << ","
             << request->getParticipantCount() << ","
             << requestStatusToString(request->getStatus()) << ","
             << (request->getRejectionReason().empty() ? "None" : request->getRejectionReason()) << ","
             << buildTimeInfo(request) << ","
             << buildHistoryInfo(request)
             << "\n";
    }

    file.close();
}
