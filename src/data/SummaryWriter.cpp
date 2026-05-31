#include "SummaryWriter.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include "../models/CommitteeMeetingRequest.h"

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

    std::string statusToString(RequestStatus status) {
        switch (status) {
            case RequestStatus::Pending: return "Pending";
            case RequestStatus::Approved: return "Approved";
            case RequestStatus::Rejected: return "Rejected";
            default: return "Unknown";
        }
    }

    std::string optionalText(const std::string& value) {
        return value.empty() ? "" : value;
    }

    std::string requesterName(const Request* request) {
        return request->getRequester() ? request->getRequester()->getName() : "";
    }

    std::string requesterRole(const Request* request) {
        return request->getRequester() ? request->getRequester()->getRoleName() : "";
    }

    std::string spaceName(const Allocation& allocation) {
        return allocation.getSpace() ? allocation.getSpace()->getName() : "";
    }

    std::string spaceType(const Allocation& allocation) {
        return allocation.getSpace() ? allocation.getSpace()->getType() : "";
    }

    std::string spaceBuilding(const Allocation& allocation) {
        return allocation.getSpace() ? allocation.getSpace()->getBuilding() : "";
    }

    int spaceCapacity(const Allocation& allocation) {
        return allocation.getSpace() ? allocation.getSpace()->getCapacity() : 0;
    }

    std::string requiredParticipantsToString(const Request* request) {
        const CommitteeMeetingRequest* committee =
            dynamic_cast<const CommitteeMeetingRequest*>(request);

        if (!committee) {
            return "";
        }

        const std::vector<int>& participantIds = committee->getRequiredParticipantIds();
        const std::vector<std::string>& participantRoles = committee->getParticipantRoles();

        std::string result;
        for (size_t i = 0; i < participantIds.size(); ++i) {
            result += std::to_string(participantIds[i]);
            result += ":";
            result += i < participantRoles.size() ? participantRoles[i] : "Participant";

            if (i + 1 < participantIds.size()) {
                result += " | ";
            }
        }

        return result;
    }

    const Request* findRequestById(const std::vector<Request*>& requests, int requestId) {
        for (const Request* request : requests) {
            if (request && request->getId() == requestId) {
                return request;
            }
        }
        return nullptr;
    }

    std::unordered_map<int, std::vector<const Allocation*>>
    groupAllocationsByRequest(const std::vector<Allocation>& allocations) {
        std::unordered_map<int, std::vector<const Allocation*>> grouped;

        for (const Allocation& allocation : allocations) {
            grouped[allocation.getRequestId()].push_back(&allocation);
        }

        return grouped;
    }

    int totalAssignedParticipants(const std::vector<const Allocation*>& allocations) {
        int total = 0;
        for (const Allocation* allocation : allocations) {
            total += allocation->getAssignedParticipants();
        }
        return total;
    }

    std::string allocatedSpacesToString(const std::vector<const Allocation*>& allocations) {
        std::string result;

        for (size_t i = 0; i < allocations.size(); ++i) {
            result += spaceName(*allocations[i]);

            if (i + 1 < allocations.size()) {
                result += " | ";
            }
        }

        return result;
    }

    std::string requestSummaryText(const Request* request,
                                   const std::vector<const Allocation*>& allocations) {
        std::ostringstream summary;
        const std::string status = statusToString(request->getStatus());

        if (request->getStatus() == RequestStatus::Approved) {
            summary << status << " with " << allocations.size() << " allocation(s)";
            const std::string spaces = allocatedSpacesToString(allocations);
            if (!spaces.empty()) {
                summary << " in " << spaces;
            }
            return summary.str();
        }

        if (request->getStatus() == RequestStatus::Rejected) {
            summary << status;
            if (!request->getRejectionReason().empty()) {
                summary << ": " << request->getRejectionReason();
            }
            return summary.str();
        }

        return status;
    }
}

void SummaryWriter::writeRequestSummaries(
    const std::string& filename,
    const std::vector<Request*>& requests,
    const std::vector<Allocation>& allocations
) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open request summary file: " << filename << "\n";
        return;
    }

    const auto allocationsByRequest = groupAllocationsByRequest(allocations);

    file << "requestId,requestType,title,purpose,requesterName,requesterRole,priority,status,rejectionReason,requiredParticipants,assignedParticipants,allocationCount,allocatedSpaces,requiredFeature,requiredBuilding,summaryText\n";

    for (const Request* request : requests) {
        if (!request) {
            continue;
        }

        std::vector<const Allocation*> requestAllocations;
        const auto allocationIt = allocationsByRequest.find(request->getId());
        if (allocationIt != allocationsByRequest.end()) {
            requestAllocations = allocationIt->second;
        }

        file << request->getId() << ","
             << escapeCsv(request->getRequestType()) << ","
             << escapeCsv(request->getTitle()) << ","
             << escapeCsv(request->getPurpose()) << ","
             << escapeCsv(requesterName(request)) << ","
             << escapeCsv(requesterRole(request)) << ","
             << request->getPriority() << ","
             << escapeCsv(statusToString(request->getStatus())) << ","
             << escapeCsv(request->getRejectionReason()) << ","
             << escapeCsv(requiredParticipantsToString(request)) << ","
             << totalAssignedParticipants(requestAllocations) << ","
             << requestAllocations.size() << ","
             << escapeCsv(allocatedSpacesToString(requestAllocations)) << ","
             << escapeCsv(optionalText(request->getRequiredFeature())) << ","
             << escapeCsv(optionalText(request->getRequiredBuilding())) << ","
             << escapeCsv(requestSummaryText(request, requestAllocations))
             << "\n";
    }
}

void SummaryWriter::writeAllocationSummaries(
    const std::string& filename,
    const std::vector<Request*>& requests,
    const std::vector<Allocation>& allocations
) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open allocation summary file: " << filename << "\n";
        return;
    }

    file << "allocationId,requestId,requestTitle,requestPurpose,requestType,spaceName,spaceType,building,day,startTime,endTime,assignedParticipants,spaceCapacity\n";

    for (const Allocation& allocation : allocations) {
        const Request* request = findRequestById(requests, allocation.getRequestId());

        file << allocation.getId() << ","
             << allocation.getRequestId() << ","
             << escapeCsv(request ? request->getTitle() : "") << ","
             << escapeCsv(request ? request->getPurpose() : "") << ","
             << escapeCsv(request ? request->getRequestType() : "Unknown") << ","
             << escapeCsv(spaceName(allocation)) << ","
             << escapeCsv(spaceType(allocation)) << ","
             << escapeCsv(spaceBuilding(allocation)) << ","
             << allocation.getTimeSlot().getDay() << ","
             << escapeCsv(allocation.getTimeSlot().getStartTimeString()) << ","
             << escapeCsv(allocation.getTimeSlot().getEndTimeString()) << ","
             << allocation.getAssignedParticipants() << ","
             << spaceCapacity(allocation)
             << "\n";
    }
}
