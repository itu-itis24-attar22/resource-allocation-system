#include "test_framework.h"
#include "test_helpers.h"
#include "../src/models/Classroom.h"
#include "../src/models/CommitteeMeetingRequest.h"
#include "../src/models/Instructor.h"
#include "../src/models/Laboratory.h"
#include "../src/models/MeetingRoom.h"
#include "../src/models/OneTimeRequest.h"
#include "../src/models/RecurringRequest.h"
#include "../src/models/Student.h"
#include "../src/models/UserBusySlot.h"
#include "../src/services/AllocationService.h"

TEST_CASE("AllocationService approves a valid one-time request") {
    Student student(1, "Student");
    Classroom room(101, "B201", 40, true, true, false, true, "Engineering");
    AllocationService service;

    OneTimeRequest request(1, &student, &room, TimeSlot(2, 13, 15),
                           20, "Project", "Meeting", "Projector", "Engineering");

    bool approved = service.processRequest(request);

    CHECK(approved);
    CHECK(request.getStatus() == RequestStatus::Approved);
    CHECK_EQ(service.getAllocations().size(), static_cast<size_t>(1));
    CHECK_EQ(service.getAllocations()[0].getRequestId(), 1);
}

TEST_CASE("AllocationService stores clear rejection reason") {
    Instructor instructor(2, "Dr. Instructor");
    Laboratory lab(201, "L101", 25, false, false, true, false, "LabBuilding");
    AllocationService service;

    OneTimeRequest request(2, &instructor, &lab, TimeSlot(2, 13, 15),
                           20, "Lab", "Meeting", "Computers", "LabBuilding");

    bool approved = service.processRequest(request);

    CHECK(!approved);
    CHECK(request.getStatus() == RequestStatus::Rejected);
    CHECK_EQ(request.getRejectionReason(), std::string("Space under maintenance"));
    CHECK(service.getAllocations().empty());
}

TEST_CASE("AllocationService approves a valid recurring request with two allocations") {
    Student student(1, "Student");
    MeetingRoom room(301, "M301", 12, true, true, false, true, "AdminBuilding");
    AllocationService service;

    RecurringRequest request(
        3,
        &student,
        &room,
        {TimeSlot(2, 9, 10), TimeSlot(4, 9, 10)},
        8,
        "Weekly Research Meeting",
        "Meeting",
        "Projector",
        "AdminBuilding"
    );

    bool approved = service.processRequest(request);

    CHECK(approved);
    CHECK(request.getStatus() == RequestStatus::Approved);
    CHECK_EQ(service.getAllocations().size(), static_cast<size_t>(2));
}

TEST_CASE("AllocationService approves committee meeting when participants are free") {
    Student student(1, "Student");
    Instructor professor(2, "Dr. Free");
    MeetingRoom room(301, "M301", 10, true, false, false, true, "AdminBuilding");
    std::vector<User*> users{&student, &professor};
    std::vector<UserBusySlot> busySlots;
    AllocationService service("greedy", users, busySlots);

    CommitteeMeetingRequest request(40, &student, &room, TimeSlot(5, 13, 14),
                                    8, "Master Thesis Presentation",
                                    "Presentation", "Projector", "AdminBuilding");
    request.addRequiredParticipant(2, "Supervisor");

    bool approved = service.processRequest(request);

    CHECK(approved);
    CHECK(request.getStatus() == RequestStatus::Approved);
    CHECK_EQ(service.getAllocations().size(), static_cast<size_t>(1));
    CHECK_EQ(service.getAllocations()[0].getRequestId(), 40);
}

TEST_CASE("AllocationService rejects busy committee participant and appends suggestions") {
    Student student(1, "Student");
    Instructor professor(2, "Dr. Busy");
    MeetingRoom room(301, "M301", 10, true, false, false, true, "AdminBuilding");
    std::vector<User*> users{&student, &professor};
    std::vector<UserBusySlot> busySlots{
        UserBusySlot(2, TimeSlot::fromMinutes(1, 10 * 60, 12 * 60), "Lecture")
    };
    AllocationService service("greedy", users, busySlots);

    CommitteeMeetingRequest request(41, &student, &room,
                                    TimeSlot::fromMinutes(1, 10 * 60, 11 * 60),
                                    8, "Busy Committee Meeting",
                                    "Presentation", "Projector", "AdminBuilding");
    request.addRequiredParticipant(2, "Supervisor");

    bool approved = service.processRequest(request);

    CHECK(!approved);
    CHECK(request.getStatus() == RequestStatus::Rejected);
    CHECK_CONTAINS(request.getRejectionReason(), "Dr. Busy");
    CHECK(service.getAllocations().empty());
    CHECK(historyContains(request, "Suggested least-change alternative times"));
    CHECK(historyContains(request, "1) Monday 09:00-10:00"));
}

TEST_CASE("AllocationService rejects committee meeting with no participants") {
    Student student(1, "Student");
    MeetingRoom room(301, "M301", 10, true, false, false, true, "AdminBuilding");
    std::vector<User*> users{&student};
    std::vector<UserBusySlot> busySlots;
    AllocationService service("greedy", users, busySlots);

    CommitteeMeetingRequest request(42, &student, &room, TimeSlot(5, 13, 14),
                                    8, "No Participants",
                                    "Presentation", "Projector", "AdminBuilding");

    bool approved = service.processRequest(request);

    CHECK(!approved);
    CHECK(request.getStatus() == RequestStatus::Rejected);
    CHECK_EQ(request.getRejectionReason(),
             std::string("Committee meeting has no required participants"));
    CHECK(!historyContains(request, "Suggested least-change alternative times"));
}

TEST_CASE("All allocation strategies process a simple one-time request without crashing") {
    const std::vector<std::string> strategies{
        "greedy",
        "priority",
        "multi_room_exam_greedy",
        "multi_room_exam_best_fit",
        "shared_room_exam_best_fit"
    };

    for (const std::string& strategyName : strategies) {
        Student student(1, "Student");
        MeetingRoom room(301, "M301", 10, true, false, false, true, "AdminBuilding");
        std::vector<User*> users{&student};
        std::vector<UserBusySlot> busySlots;
        AllocationService service(strategyName, users, busySlots);
        OneTimeRequest request(100, &student, &room, TimeSlot(2, 9, 10),
                               4, "Strategy Smoke", "Meeting",
                               "Projector", "AdminBuilding");

        bool approved = service.processRequest(request);

        CHECK(approved);
        CHECK(request.getStatus() == RequestStatus::Approved);
        CHECK_EQ(service.getAllocations().size(), static_cast<size_t>(1));
    }
}
