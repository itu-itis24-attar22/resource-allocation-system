#include <iostream>
#include <string>
#include "models/User.h"
#include "models/Classroom.h"
#include "models/Laboratory.h"
#include "models/TimeSlot.h"
#include "models/OneTimeRequest.h"
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

void printRequestResult(
    const std::string& label,
    const OneTimeRequest& request,
    bool result,
    const std::string& explanation
) {
    std::cout << label << "\n";
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

int main() {
    User user1(1, "Najib", "Student");

    Classroom classroomA(101, "B201", 40);
    Laboratory labA(201, "L101", 25);

    AllocationService allocationService;

    Allocation existingClassroomAllocation(100, 999, &classroomA, TimeSlot(10, 12));
    allocationService.addExistingAllocation(existingClassroomAllocation);

    // Case 1: Classroom approved
    OneTimeRequest request1(1, user1, &classroomA, TimeSlot(13, 15), 30);
    bool result1 = allocationService.processRequest(request1);
    printRequestResult("Request 1", request1, result1, "Classroom approved");

    // Case 2: Laboratory approved
    OneTimeRequest request2(2, user1, &labA, TimeSlot(10, 12), 20);
    bool result2 = allocationService.processRequest(request2);
    printRequestResult("Request 2", request2, result2, "Laboratory approved");

    // Case 3: Laboratory rejected by capacity
    OneTimeRequest request3(3, user1, &labA, TimeSlot(13, 15), 30);
    bool result3 = allocationService.processRequest(request3);
    printRequestResult("Request 3", request3, result3, "Laboratory rejected by capacity");

    // Case 4: Classroom rejected by availability
    OneTimeRequest request4(4, user1, &classroomA, TimeSlot(11, 13), 20);
    bool result4 = allocationService.processRequest(request4);
    printRequestResult("Request 4", request4, result4, "Classroom rejected by availability");

    allocationService.printAllocations();

    return 0;
}