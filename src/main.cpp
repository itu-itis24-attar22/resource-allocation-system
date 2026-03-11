#include <iostream>
#include <string>
#include "User.h"
#include "Classroom.h"
#include "TimeSlot.h"
#include "OneTimeRequest.h"
#include "Allocation.h"
#include "AllocationService.h"
#include "Status.h"

std::string requestStatusToString(RequestStatus status) {
    switch (status) {
        case RequestStatus::Pending: return "Pending";
        case RequestStatus::Approved: return "Approved";
        case RequestStatus::Rejected: return "Rejected";
        default: return "Unknown";
    }
}

int main() {
    User user1(1, "Najib", "Student");
    Classroom roomA(101, "B201", 40);

    AllocationService allocationService;

    Allocation existingAllocation(100, 999, roomA, TimeSlot(10, 12));
    allocationService.addExistingAllocation(existingAllocation);

    OneTimeRequest request1(1, user1, roomA, TimeSlot(11, 13));
    bool result1 = allocationService.processRequest(request1);

    std::cout << "Request 1 status: " << requestStatusToString(request1.getStatus()) << "\n";
    std::cout << (result1 ? "Allocation created.\n" : "Request rejected due to conflict.\n");

    OneTimeRequest request2(2, user1, roomA, TimeSlot(13, 15));
    bool result2 = allocationService.processRequest(request2);

    std::cout << "\nRequest 2 status: " << requestStatusToString(request2.getStatus()) << "\n";
    std::cout << (result2 ? "Allocation created.\n" : "Request rejected due to conflict.\n");

    allocationService.printAllocations();

    return 0;
}