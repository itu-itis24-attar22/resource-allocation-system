#include "test_framework.h"
#include "../src/models/Allocation.h"
#include "../src/models/CommitteeMeetingRequest.h"
#include "../src/models/Instructor.h"
#include "../src/models/MeetingRoom.h"
#include "../src/models/Student.h"
#include "../src/models/UserBusySlot.h"
#include "../src/services/MeetingTimeSuggestionService.h"

TEST_CASE("MeetingTimeSuggestionService ranks valid slots by least time change") {
    Student student(1, "Student");
    Instructor professor(2, "Dr. Busy");
    MeetingRoom room(301, "M301", 10, true, false, false, true, "AdminBuilding");
    CommitteeMeetingRequest request(41, &student, &room,
                                    TimeSlot::fromMinutes(1, 10 * 60, 11 * 60),
                                    8, "Busy Committee Meeting",
                                    "Presentation", "Projector", "AdminBuilding");
    request.addRequiredParticipant(2, "Supervisor");

    std::vector<UserBusySlot> busySlots{
        UserBusySlot(2, TimeSlot::fromMinutes(1, 10 * 60, 12 * 60), "Lecture")
    };
    std::vector<Allocation> allocations;
    MeetingTimeSuggestionService service;

    std::vector<MeetingTimeSuggestion> suggestions =
        service.suggestTimes(request, &room, busySlots, allocations, 3);

    CHECK_EQ(suggestions.size(), static_cast<size_t>(3));
    CHECK_EQ(suggestions[0].getTimeSlot().getDay(), 1);
    CHECK_EQ(suggestions[0].getTimeSlot().getStartTimeString(), std::string("09:00"));
    CHECK_EQ(suggestions[0].getTimeDistanceMinutes(), 60);
    CHECK_EQ(suggestions[1].getTimeSlot().getStartTimeString(), std::string("12:00"));
    CHECK_EQ(suggestions[1].getTimeDistanceMinutes(), 120);
    CHECK_EQ(suggestions[2].getTimeSlot().getStartTimeString(), std::string("12:30"));
    CHECK_EQ(suggestions[2].getTimeDistanceMinutes(), 150);
}

TEST_CASE("MeetingTimeSuggestionService excludes requested-room conflicts") {
    Student student(1, "Student");
    MeetingRoom room(301, "M301", 10, true, false, false, true, "AdminBuilding");
    CommitteeMeetingRequest request(41, &student, &room,
                                    TimeSlot::fromMinutes(1, 10 * 60, 11 * 60),
                                    8, "Busy Committee Meeting",
                                    "Presentation", "Projector", "AdminBuilding");
    request.addRequiredParticipant(2, "Supervisor");

    std::vector<UserBusySlot> busySlots{
        UserBusySlot(2, TimeSlot::fromMinutes(1, 10 * 60, 12 * 60), "Lecture")
    };
    std::vector<Allocation> allocations{
        Allocation(1, 99, &room, TimeSlot::fromMinutes(1, 9 * 60, 10 * 60), 4)
    };
    MeetingTimeSuggestionService service;

    std::vector<MeetingTimeSuggestion> suggestions =
        service.suggestTimes(request, &room, busySlots, allocations, 3);

    CHECK_EQ(suggestions.size(), static_cast<size_t>(3));
    CHECK_EQ(suggestions[0].getTimeSlot().getStartTimeString(), std::string("12:00"));
    CHECK_EQ(suggestions[0].getTimeDistanceMinutes(), 120);
}

TEST_CASE("MeetingTimeSuggestionService returns no suggestions without required participants") {
    Student student(1, "Student");
    MeetingRoom room(301, "M301", 10, true, false, false, true, "AdminBuilding");
    CommitteeMeetingRequest request(42, &student, &room, TimeSlot(5, 13, 14),
                                    8, "No Participants",
                                    "Presentation", "Projector", "AdminBuilding");
    MeetingTimeSuggestionService service;

    std::vector<MeetingTimeSuggestion> suggestions =
        service.suggestTimes(request, &room, {}, {}, 3);

    CHECK(suggestions.empty());
}
