#include <iostream>
#include "ConsolePrinter.h"

std::string requestStatusToString(RequestStatus status) {
    switch (status) {
        case RequestStatus::Pending: return "Pending";
        case RequestStatus::Approved: return "Approved";
        case RequestStatus::Rejected: return "Rejected";
        default: return "Unknown";
    }
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

std::string availabilityToString(bool availability) {
    return availability ? "Available" : "UnderMaintenance";
}

void printOneTimeResult(
    const std::string& label,
    const OneTimeRequest& request,
    bool result,
    const std::string& explanation
) {
    std::cout << label << "\n";
    std::cout << "Request type: OneTimeRequest\n";
    std::cout << "Title: " << request.getTitle() << "\n";
    std::cout << "Purpose: " << request.getPurpose() << "\n";
    std::cout << "Requester: " << request.getRequester()->getName() << "\n";
    std::cout << "Requester role: " << request.getRequester()->getRoleName() << "\n";
    std::cout << "Priority: " << request.getPriority() << "\n";
    std::cout << "Requested type: " << request.getRequestedSpace()->getType() << "\n";
    std::cout << "Requested space: " << request.getRequestedSpace()->getName() << "\n";
    std::cout << "Space building: " << request.getRequestedSpace()->getBuilding() << "\n";
    std::cout << "Required building: "
              << (request.getRequiredBuilding().empty() ? "None" : request.getRequiredBuilding()) << "\n";
    std::cout << "Space status: " << availabilityToString(request.getRequestedSpace()->getIsAvailable()) << "\n";
    std::cout << "Required feature: "
              << (request.getRequiredFeature().empty() ? "None" : request.getRequiredFeature()) << "\n";
    std::cout << "Status: " << requestStatusToString(request.getStatus()) << "\n";
    std::cout << "Participants: " << request.getParticipantCount() << "\n";
    std::cout << "Requested time: "
              << dayToString(request.getRequestedTimeSlot().getDay()) << ", "
              << request.getRequestedTimeSlot().getStartTimeString() << " - "
              << request.getRequestedTimeSlot().getEndTimeString() << "\n";

    if (result) {
        std::cout << "Allocation created.\n";
    } else {
        std::cout << "Request rejected.\n";
        std::cout << "Rejection reason: " << request.getRejectionReason() << "\n";
    }

    std::cout << "Case: " << explanation << "\n";
    std::cout << "-----------------------------------\n";
}

void printRecurringResult(
    const std::string& label,
    const RecurringRequest& request,
    bool result,
    const std::string& explanation
) {
    std::cout << label << "\n";
    std::cout << "Request type: RecurringRequest\n";
    std::cout << "Title: " << request.getTitle() << "\n";
    std::cout << "Purpose: " << request.getPurpose() << "\n";
    std::cout << "Requester: " << request.getRequester()->getName() << "\n";
    std::cout << "Requester role: " << request.getRequester()->getRoleName() << "\n";
    std::cout << "Priority: " << request.getPriority() << "\n";
    std::cout << "Requested type: " << request.getRequestedSpace()->getType() << "\n";
    std::cout << "Requested space: " << request.getRequestedSpace()->getName() << "\n";
    std::cout << "Space building: " << request.getRequestedSpace()->getBuilding() << "\n";
    std::cout << "Required building: "
              << (request.getRequiredBuilding().empty() ? "None" : request.getRequiredBuilding()) << "\n";
    std::cout << "Space status: " << availabilityToString(request.getRequestedSpace()->getIsAvailable()) << "\n";
    std::cout << "Required feature: "
              << (request.getRequiredFeature().empty() ? "None" : request.getRequiredFeature()) << "\n";
    std::cout << "Status: " << requestStatusToString(request.getStatus()) << "\n";
    std::cout << "Participants: " << request.getParticipantCount() << "\n";
    std::cout << "Occurrences: " << request.getRequestedTimeSlots().size() << "\n";

    const std::vector<TimeSlot>& slots = request.getRequestedTimeSlots();
    for (size_t i = 0; i < slots.size(); i++) {
        std::cout << "  Occurrence " << i + 1 << ": "
                  << dayToString(slots[i].getDay()) << ", "
                  << slots[i].getStartTimeString() << " - "
                  << slots[i].getEndTimeString() << "\n";
    }

    if (result) {
        std::cout << "Allocations created for all occurrences.\n";
    } else {
        std::cout << "Recurring request rejected.\n";
        std::cout << "Rejection reason: " << request.getRejectionReason() << "\n";
    }

    std::cout << "Case: " << explanation << "\n";
    std::cout << "-----------------------------------\n";
}

void printExamResult(
    const std::string& label,
    const ExamRequest& request,
    bool result,
    const std::string& explanation
) {
    std::cout << label << "\n";
    std::cout << "Request type: ExamRequest\n";
    std::cout << "Title: " << request.getTitle() << "\n";
    std::cout << "Purpose: " << request.getPurpose() << "\n";
    std::cout << "Course code: " << request.getCourseCode() << "\n";
    std::cout << "Course name: " << request.getCourseName() << "\n";
    std::cout << "Exam type: " << request.getExamType() << "\n";
    std::cout << "Can split across rooms: " << (request.getCanSplitAcrossRooms() ? "Yes" : "No") << "\n";
    std::cout << "Requester: " << request.getRequester()->getName() << "\n";
    std::cout << "Requester role: " << request.getRequester()->getRoleName() << "\n";
    std::cout << "Priority: " << request.getPriority() << "\n";
    std::cout << "Requested type: " << request.getRequestedSpace()->getType() << "\n";
    std::cout << "Requested space: " << request.getRequestedSpace()->getName() << "\n";
    std::cout << "Space building: " << request.getRequestedSpace()->getBuilding() << "\n";
    std::cout << "Required building: "
              << (request.getRequiredBuilding().empty() ? "None" : request.getRequiredBuilding()) << "\n";
    std::cout << "Space status: " << availabilityToString(request.getRequestedSpace()->getIsAvailable()) << "\n";
    std::cout << "Required feature: "
              << (request.getRequiredFeature().empty() ? "None" : request.getRequiredFeature()) << "\n";
    std::cout << "Status: " << requestStatusToString(request.getStatus()) << "\n";
    std::cout << "Participants: " << request.getParticipantCount() << "\n";
    std::cout << "Exam time: "
              << dayToString(request.getExamTimeSlot().getDay()) << ", "
              << request.getExamTimeSlot().getStartTimeString() << " - "
              << request.getExamTimeSlot().getEndTimeString() << "\n";

    if (result) {
        std::cout << "Allocation created.\n";
    } else {
        std::cout << "Request rejected.\n";
        std::cout << "Rejection reason: " << request.getRejectionReason() << "\n";
    }

    std::cout << "Case: " << explanation << "\n";
    std::cout << "-----------------------------------\n";
}

void printInvalidResult(
    const std::string& label,
    const InvalidRequest& request,
    const std::string& explanation
) {
    std::cout << label << "\n";
    std::cout << "Request type: InvalidRequest\n";
    std::cout << "Title: " << request.getTitle() << "\n";
    std::cout << "Purpose: " << request.getPurpose() << "\n";
    std::cout << "Requester: " << request.getRequester()->getName() << "\n";
    std::cout << "Requester role: " << request.getRequester()->getRoleName() << "\n";
    std::cout << "Priority: " << request.getPriority() << "\n";
    std::cout << "Requested type: " << request.getRequestedSpace()->getType() << "\n";
    std::cout << "Requested space: " << request.getRequestedSpace()->getName() << "\n";
    std::cout << "Space building: " << request.getRequestedSpace()->getBuilding() << "\n";
    std::cout << "Required building: "
              << (request.getRequiredBuilding().empty() ? "None" : request.getRequiredBuilding()) << "\n";
    std::cout << "Required feature: "
              << (request.getRequiredFeature().empty() ? "None" : request.getRequiredFeature()) << "\n";
    std::cout << "Status: " << requestStatusToString(request.getStatus()) << "\n";
    std::cout << "Participants: " << request.getParticipantCount() << "\n";
    std::cout << "Raw time input: "
              << (request.getRawTimeInfo().empty() ? "None" : request.getRawTimeInfo()) << "\n";
    std::cout << "Request rejected.\n";
    std::cout << "Rejection reason: " << request.getRejectionReason() << "\n";
    std::cout << "Case: " << explanation << "\n";
    std::cout << "-----------------------------------\n";
}
