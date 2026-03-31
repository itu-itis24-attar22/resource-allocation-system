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
    std::cout << "Status: " << requestStatusToString(request.getStatus()) << "\n";
    std::cout << "Participants: " << request.getParticipantCount() << "\n";
    std::cout << "Requested time: "
              << request.getRequestedTimeSlot().getStartHour() << ":00 - "
              << request.getRequestedTimeSlot().getEndHour() << ":00\n";
    std::cout << (result ? "Allocation created.\n" : "Request rejected.\n");
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
    std::cout << "Status: " << requestStatusToString(request.getStatus()) << "\n";
    std::cout << "Participants: " << request.getParticipantCount() << "\n";
    std::cout << "Occurrences: " << request.getRequestedTimeSlots().size() << "\n";
    std::cout << (result ? "Allocations created for all occurrences.\n" : "Recurring request rejected.\n");
    std::cout << "Case: " << explanation << "\n";
    std::cout << "-----------------------------------\n";
}

int main() {
    User user1(1, "Najib", "Student");

    Classroom classroomA(101, "B201", 40);
    Laboratory labA(201, "L101", 25);
    MeetingRoom meetingRoomA(301, "M301", 12);

    AllocationService allocationService;

    Allocation existingClassroomAllocation(100, 999, &classroomA, TimeSlot(10, 12));
    Allocation existingLabAllocation(101, 998, &labA, TimeSlot(14, 16));
    allocationService.addExistingAllocation(existingClassroomAllocation);
    allocationService.addExistingAllocation(existingLabAllocation);

    // Case 1: One-time classroom approved
    OneTimeRequest request1(1, user1, &classroomA, TimeSlot(13, 15), 30);
    bool result1 = allocationService.processRequest(request1);
    printOneTimeResult("Request 1", request1, result1, "One-time classroom approved");

    // Case 2: Recurring meeting room approved
    RecurringRequest request2(
        2, user1, &meetingRoomA,
        std::vector<TimeSlot>{TimeSlot(9, 10), TimeSlot(10, 11), TimeSlot(11, 12)},
        8
    );
    bool result2 = allocationService.processRequest(request2);
    printRecurringResult("Request 2", request2, result2, "Recurring meeting room approved");

    // Case 3: Recurring laboratory rejected by availability
    RecurringRequest request3(
        3, user1, &labA,
        std::vector<TimeSlot>{TimeSlot(13, 14), TimeSlot(14, 15), TimeSlot(15, 16)},
        20
    );
    bool result3 = allocationService.processRequest(request3);
    printRecurringResult("Request 3", request3, result3, "Recurring laboratory rejected by availability");

    // Case 4: Recurring meeting room rejected by capacity
    RecurringRequest request4(
        4, user1, &meetingRoomA,
        std::vector<TimeSlot>{TimeSlot(12, 13), TimeSlot(13, 14)},
        20
    );
    bool result4 = allocationService.processRequest(request4);
    printRecurringResult("Request 4", request4, result4, "Recurring meeting room rejected by capacity");

    // Case 5: One-time classroom rejected by availability
    OneTimeRequest request5(5, user1, &classroomA, TimeSlot(11, 13), 20);
    bool result5 = allocationService.processRequest(request5);
    printOneTimeResult("Request 5", request5, result5, "One-time classroom rejected by availability");

    allocationService.printAllocations();

    return 0;
}