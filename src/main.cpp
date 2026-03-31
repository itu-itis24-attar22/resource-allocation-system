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
    std::cout << "Required feature: " 
              << (request.getRequiredFeature().empty() ? "None" : request.getRequiredFeature()) << "\n";
    std::cout << "Status: " << requestStatusToString(request.getStatus()) << "\n";
    std::cout << "Participants: " << request.getParticipantCount() << "\n";
    std::cout << "Requested time: "
              << dayToString(request.getRequestedTimeSlot().getDay()) << ", "
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

    Classroom classroomA(101, "B201", 40, true, true, false);
    Laboratory labA(201, "L101", 25, false, true, true);
    MeetingRoom meetingRoomA(301, "M301", 12, true, true, false);

    AllocationService allocationService;

    Allocation existingClassroomAllocation(100, 999, &classroomA, TimeSlot(1, 10, 12));
    Allocation existingLabAllocation(101, 998, &labA, TimeSlot(3, 14, 16));
    allocationService.addExistingAllocation(existingClassroomAllocation);
    allocationService.addExistingAllocation(existingLabAllocation);

    OneTimeRequest request1(1, user1, &classroomA, TimeSlot(2, 13, 15), 30, "Projector");
    bool result1 = allocationService.processRequest(request1);
    printOneTimeResult("Request 1", request1, result1, "One-time classroom approved because the requested space has a projector");

    RecurringRequest request2(
        2, user1, &labA,
        std::vector<TimeSlot>{TimeSlot(1, 9, 10), TimeSlot(3, 10, 11), TimeSlot(5, 11, 12)},
        20,
        "Computers"
    );
    bool result2 = allocationService.processRequest(request2);
    printRecurringResult("Request 2", request2, result2, "Recurring laboratory approved because all occurrences satisfy capacity, feature, and availability rules");

    OneTimeRequest request3(3, user1, &meetingRoomA, TimeSlot(4, 10, 11), 8, "Computers");
    bool result3 = allocationService.processRequest(request3);
    printOneTimeResult("Request 3", request3, result3, "One-time meeting room rejected because the requested space does not have computers");

    RecurringRequest request4(
        4, user1, &classroomA,
        std::vector<TimeSlot>{TimeSlot(2, 9, 10), TimeSlot(4, 9, 10)},
        25,
        "Whiteboard"
    );
    bool result4 = allocationService.processRequest(request4);
    printRecurringResult("Request 4", request4, result4, "Recurring classroom approved because the requested space has a whiteboard");

    OneTimeRequest request5(5, user1, &labA, TimeSlot(3, 14, 15), 20, "Whiteboard");
    bool result5 = allocationService.processRequest(request5);
    printOneTimeResult("Request 5", request5, result5, "One-time laboratory rejected because the requested time overlaps with an existing allocation");

    allocationService.printAllocations();

    return 0;
}