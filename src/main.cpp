#include <iostream>
#include <string>
#include <vector>
#include "models/User.h"
#include "models/Classroom.h"
#include "models/Laboratory.h"
#include "models/MeetingRoom.h"
#include "models/TimeSlot.h"
#include "models/OneTimeRequest.h"
#include "models/RecurringRequest.h"
#include "models/Allocation.h"
#include "services/AllocationService.h"
#include "models/Status.h"

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
    std::cout << "Requested type: " << request.getRequestedSpace()->getType() << "\n";
    std::cout << "Requested space: " << request.getRequestedSpace()->getName() << "\n";
    std::cout << "Space status: " << availabilityToString(request.getRequestedSpace()->getIsAvailable()) << "\n";
    std::cout << "Required feature: "
              << (request.getRequiredFeature().empty() ? "None" : request.getRequiredFeature()) << "\n";
    std::cout << "Status: " << requestStatusToString(request.getStatus()) << "\n";
    std::cout << "Participants: " << request.getParticipantCount() << "\n";
    std::cout << "Requested time: "
              << dayToString(request.getRequestedTimeSlot().getDay()) << ", "
              << request.getRequestedTimeSlot().getStartHour() << ":00 - "
              << request.getRequestedTimeSlot().getEndHour() << ":00\n";

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
    std::cout << "Requested type: " << request.getRequestedSpace()->getType() << "\n";
    std::cout << "Requested space: " << request.getRequestedSpace()->getName() << "\n";
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
                  << slots[i].getStartHour() << ":00 - "
                  << slots[i].getEndHour() << ":00\n";
    }

    std::cout << (result ? "Allocations created for all occurrences.\n" : "Recurring request rejected.\n");
    std::cout << "Case: " << explanation << "\n";
    std::cout << "-----------------------------------\n";
}

int main() {
    User user1(1, "Najib", "Student");

    Classroom classroomA(101, "B201", 40, true, true, false, true);
    Laboratory labA(201, "L101", 25, false, true, true, false);
    MeetingRoom meetingRoomA(301, "M301", 12, true, true, false, true);

    AllocationService allocationService;

    Allocation existingClassroomAllocation(100, 999, &classroomA, TimeSlot(1, 10, 12));
    allocationService.addExistingAllocation(existingClassroomAllocation);

    OneTimeRequest request1(1, user1, &classroomA, TimeSlot(2, 13, 15), 30, "Projector");
    bool result1 = allocationService.processRequest(request1);
    printOneTimeResult("Request 1", request1, result1, "One-time classroom approved because the space is available and satisfies all rules");

    OneTimeRequest request2(2, user1, &labA, TimeSlot(3, 10, 12), 20, "Computers");
    bool result2 = allocationService.processRequest(request2);
    printOneTimeResult("Request 2", request2, result2, "One-time laboratory rejected because the space is under maintenance");

    RecurringRequest request3(
        3, user1, &meetingRoomA,
        std::vector<TimeSlot>{TimeSlot(2, 9, 10), TimeSlot(4, 9, 10)},
        8,
        "Projector"
    );
    bool result3 = allocationService.processRequest(request3);
    printRecurringResult("Request 3", request3, result3, "Recurring meeting room approved because the space is available and all occurrences satisfy the rules");

    RecurringRequest request4(
        4, user1, &labA,
        std::vector<TimeSlot>{TimeSlot(1, 11, 12), TimeSlot(5, 11, 12)},
        15,
        "Whiteboard"
    );
    bool result4 = allocationService.processRequest(request4);
    printRecurringResult("Request 4", request4, result4, "Recurring laboratory rejected because the space is under maintenance");

    OneTimeRequest request5(5, user1, &classroomA, TimeSlot(1, 11, 13), 20, "Whiteboard");
    bool result5 = allocationService.processRequest(request5);
    printOneTimeResult("Request 5", request5, result5, "One-time classroom rejected because the requested time overlaps with an existing allocation");

    allocationService.printAllocations();

    return 0;
}