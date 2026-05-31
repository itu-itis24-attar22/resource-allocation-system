#include "test_framework.h"
#include "../src/models/UserBusySlot.h"
#include "../src/services/UserAvailabilityService.h"

TEST_CASE("UserAvailabilityService applies working hours and busy-slot overlaps") {
    UserAvailabilityService service;
    std::vector<UserBusySlot> busySlots{
        UserBusySlot(2, TimeSlot::fromMinutes(1, 10 * 60, 12 * 60), "Lecture")
    };

    CHECK(service.isUserAvailable(2, TimeSlot::fromMinutes(1, 9 * 60, 10 * 60), busySlots));
    CHECK(!service.isUserAvailable(2, TimeSlot::fromMinutes(1, 10 * 60, 11 * 60), busySlots));
    CHECK(!service.isUserAvailable(2, TimeSlot::fromMinutes(1, 11 * 60 + 30, 12 * 60 + 30), busySlots));
    CHECK(service.isUserAvailable(2, TimeSlot::fromMinutes(1, 12 * 60, 13 * 60), busySlots));
    CHECK(service.isUserAvailable(2, TimeSlot::fromMinutes(1, 9 * 60 + 30, 10 * 60), busySlots));
    CHECK(!service.isUserAvailable(2, TimeSlot::fromMinutes(1, 9 * 60 + 30, 10 * 60 + 30), busySlots));
    CHECK(!service.isUserAvailable(2, TimeSlot::fromMinutes(1, 17 * 60, 18 * 60), busySlots));
    CHECK(!service.isUserAvailable(2, TimeSlot::fromMinutes(1, 8 * 60 + 30, 9 * 60 + 30), busySlots));
    CHECK(!service.isUserAvailable(2, TimeSlot::fromMinutes(6, 10 * 60, 11 * 60), busySlots));
    CHECK(!service.isUserAvailable(2, TimeSlot::fromMinutes(7, 10 * 60, 11 * 60), busySlots));
    CHECK(service.isUserAvailable(3, TimeSlot::fromMinutes(1, 10 * 60, 11 * 60), busySlots));
}

TEST_CASE("UserAvailabilityService exposes busy slots for one user only") {
    UserAvailabilityService service;
    std::vector<UserBusySlot> busySlots{
        UserBusySlot(2, TimeSlot(1, 10, 11), "Lecture"),
        UserBusySlot(3, TimeSlot(1, 10, 11), "Office hour"),
        UserBusySlot(2, TimeSlot(2, 14, 15), "Meeting")
    };

    std::vector<UserBusySlot> user2Slots = service.getBusySlotsForUser(2, busySlots);

    CHECK_EQ(user2Slots.size(), static_cast<size_t>(2));
    CHECK_EQ(user2Slots[0].getReason(), std::string("Lecture"));
    CHECK_EQ(user2Slots[1].getReason(), std::string("Meeting"));
}
