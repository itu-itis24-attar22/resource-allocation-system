#include <iostream>
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
#include "data/DataLoader.h"
#include "utils/ConsolePrinter.h"

int main() {
    std::vector<User> users = DataLoader::loadUsers("data/users.csv");
    std::vector<Space*> spaces = DataLoader::loadSpaces("data/spaces.csv");

    if (users.size() < 4 || spaces.size() < 3) {
        std::cout << "Error: Failed to load initial data.\n";
        return 1;
    }

    User studentUser = users[0];
    User instructorUser = users[1];
    User staffUser = users[2];
    User adminUser = users[3];

    Space* classroomA = spaces[0];
    Space* labA = spaces[1];
    Space* meetingRoomA = spaces[2];

    AllocationService allocationService;

    Allocation existingClassroomAllocation(100, 999, classroomA, TimeSlot(1, 10, 12));
    allocationService.addExistingAllocation(existingClassroomAllocation);

    OneTimeRequest request1(
        1, studentUser, classroomA,
        TimeSlot(2, 13, 15),
        30,
        "Projector",
        "Engineering"
    );
    bool result1 = allocationService.processRequest(request1);
    printOneTimeResult(
        "Request 1",
        request1,
        result1,
        "Student classroom request approved; request now carries computed priority based on user role"
    );

    OneTimeRequest request2(
        2, studentUser, meetingRoomA,
        TimeSlot(3, 10, 12),
        8,
        "Projector",
        "Engineering"
    );
    bool result2 = allocationService.processRequest(request2);
    printOneTimeResult(
        "Request 2",
        request2,
        result2,
        "Student meeting room request rejected because the required building does not match"
    );

    RecurringRequest request3(
        3, instructorUser, meetingRoomA,
        std::vector<TimeSlot>{TimeSlot(2, 9, 10), TimeSlot(4, 9, 10)},
        8,
        "Projector",
        "AdminBuilding"
    );
    bool result3 = allocationService.processRequest(request3);
    printRecurringResult(
        "Request 3",
        request3,
        result3,
        "Instructor recurring request approved; instructor priority is higher than student priority"
    );

    RecurringRequest request4(
        4, studentUser, classroomA,
        std::vector<TimeSlot>{TimeSlot(2, 9, 10), TimeSlot(4, 9, 10)},
        20,
        "Whiteboard",
        "LabBuilding"
    );
    bool result4 = allocationService.processRequest(request4);
    printRecurringResult(
        "Request 4",
        request4,
        result4,
        "Student recurring classroom request rejected because the required building does not match"
    );

    OneTimeRequest request5(
        5, instructorUser, labA,
        TimeSlot(3, 14, 15),
        20,
        "Computers",
        "LabBuilding"
    );
    bool result5 = allocationService.processRequest(request5);
    printOneTimeResult(
        "Request 5",
        request5,
        result5,
        "Instructor laboratory request rejected because the space is under maintenance"
    );

    OneTimeRequest request6(
        6, studentUser, labA,
        TimeSlot(5, 9, 11),
        15,
        "Computers",
        "LabBuilding"
    );
    bool result6 = allocationService.processRequest(request6);
    printOneTimeResult(
        "Request 6",
        request6,
        result6,
        "Student laboratory request rejected because student is not authorized to request laboratory"
    );

    OneTimeRequest request7(
        7, staffUser, classroomA,
        TimeSlot(5, 12, 14),
        10,
        "Projector",
        "Engineering"
    );
    bool result7 = allocationService.processRequest(request7);
    printOneTimeResult(
        "Request 7",
        request7,
        result7,
        "Staff classroom request rejected because staff is only allowed to request meeting rooms"
    );

    OneTimeRequest request8(
        8, adminUser, meetingRoomA,
        TimeSlot(6, 10, 12),
        6,
        "Projector",
        "AdminBuilding"
    );
    bool result8 = allocationService.processRequest(request8);
    printOneTimeResult(
        "Request 8",
        request8,
        result8,
        "Administrator meeting room request approved; administrator receives the highest request priority"
    );

    allocationService.printAllocations();

    for (Space* space : spaces) {
        delete space;
    }

    return 0;
}