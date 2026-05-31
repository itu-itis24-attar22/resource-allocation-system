#include "test_framework.h"
#include "test_helpers.h"
#include "../src/models/Classroom.h"
#include "../src/models/ExamRequest.h"
#include "../src/models/Instructor.h"
#include "../src/services/AllocationService.h"

TEST_CASE("Shared-room exam strategy uses remaining capacity before spilling to another room") {
    Instructor instructor(2, "Dr. Exam");
    Classroom s401(105, "S401", 100, false, true, false, true, "SharedExamBuilding");
    Classroom b202(102, "B202", 20, false, true, false, true, "SharedExamBuilding");
    std::vector<User*> users{&instructor};
    std::vector<UserBusySlot> busySlots;
    std::vector<Space*> spaces{&s401, &b202};

    ExamRequest request7(7, &instructor, &s401, TimeSlot(5, 10, 12),
                         60, "Shared Room Exam A", "Exam", "Whiteboard",
                         "SharedExamBuilding", "PHY301",
                         "Physics Shared Seating", "Midterm", true);
    ExamRequest request8(8, &instructor, &s401, TimeSlot(5, 10, 12),
                         50, "Shared Room Exam B", "Exam", "Whiteboard",
                         "SharedExamBuilding", "PHY302",
                         "Applied Physics Shared Seating", "Midterm", true);
    std::vector<Request*> requests{&request7, &request8};
    AllocationService service("shared_room_exam_best_fit", users, busySlots);

    service.processRequests(requests, spaces);

    CHECK(request7.getStatus() == RequestStatus::Approved);
    CHECK(request8.getStatus() == RequestStatus::Approved);
    CHECK_EQ(allocationCountForRequest(service.getAllocations(), 7), 1);
    CHECK_EQ(allocationCountForRequest(service.getAllocations(), 8), 2);
    CHECK_EQ(assignedParticipantsFor(service.getAllocations(), 7, 105), 60);
    CHECK_EQ(assignedParticipantsFor(service.getAllocations(), 8, 105), 40);
    CHECK_EQ(assignedParticipantsFor(service.getAllocations(), 8, 102), 10);
}
