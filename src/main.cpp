#include <iostream>
#include <string>
#include "models/User.h"
#include "models/Classroom.h"
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
    Classroom roomA(101, "B201", 40);

    AllocationService allocationService;

    // Existing allocation: classroom already booked from 10:00 to 12:00
    Allocation existingAllocation(100, 999, roomA, TimeSlot(10, 12));
    allocationService.addExistingAllocation(existingAllocation);

    // Case 1: Availability passes, Capacity passes -> Approved
    OneTimeRequest request1(1, user1, roomA, TimeSlot(13, 15), 30);
    bool result1 = allocationService.processRequest(request1);
    printRequestResult(
        "Request 1",
        request1,
        result1,
        "Availability accepted, Capacity accepted"
    );

    // Case 2: Availability passes, Capacity fails -> Rejected
    OneTimeRequest request2(2, user1, roomA, TimeSlot(15, 17), 50);
    bool result2 = allocationService.processRequest(request2);
    printRequestResult(
        "Request 2",
        request2,
        result2,
        "Availability accepted, Capacity rejected"
    );

    // Case 3: Availability fails, Capacity passes -> Rejected
    OneTimeRequest request3(3, user1, roomA, TimeSlot(11, 13), 20);
    bool result3 = allocationService.processRequest(request3);
    printRequestResult(
        "Request 3",
        request3,
        result3,
        "Availability rejected, Capacity accepted"
    );

    // Case 4: Availability fails, Capacity fails -> Rejected
    OneTimeRequest request4(4, user1, roomA, TimeSlot(11, 12), 60);
    bool result4 = allocationService.processRequest(request4);
    printRequestResult(
        "Request 4",
        request4,
        result4,
        "Availability rejected, Capacity rejected"
    );

    allocationService.printAllocations();

    return 0;
}