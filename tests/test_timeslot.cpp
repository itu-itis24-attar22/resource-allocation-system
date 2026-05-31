#include "test_framework.h"
#include "../src/models/TimeSlot.h"

TEST_CASE("TimeSlot detects overlap on the same day") {
    TimeSlot a(1, 10, 12);
    TimeSlot b(1, 11, 13);

    CHECK(a.overlapsWith(b));
    CHECK(b.overlapsWith(a));
}

TEST_CASE("TimeSlot does not overlap across different days") {
    TimeSlot a(1, 10, 12);
    TimeSlot b(2, 10, 12);

    CHECK(!a.overlapsWith(b));
}

TEST_CASE("TimeSlot uses half-open interval boundaries") {
    TimeSlot meeting(1, 10, 11);
    TimeSlot before(1, 9, 10);
    TimeSlot after(1, 11, 12);
    TimeSlot tinyOverlapBefore = TimeSlot::fromMinutes(1, 9 * 60 + 59, 10 * 60 + 1);
    TimeSlot tinyOverlapAfter = TimeSlot::fromMinutes(1, 10 * 60 + 59, 11 * 60 + 30);

    CHECK(!meeting.overlapsWith(before));
    CHECK(!meeting.overlapsWith(after));
    CHECK(meeting.overlapsWith(tinyOverlapBefore));
    CHECK(meeting.overlapsWith(tinyOverlapAfter));
}

TEST_CASE("TimeSlot preserves minute-based times") {
    TimeSlot slot = TimeSlot::fromMinutes(3, 9 * 60 + 30, 10 * 60 + 45);

    CHECK_EQ(slot.getDay(), 3);
    CHECK_EQ(slot.getStartHour(), 9);
    CHECK_EQ(slot.getStartMinute(), 30);
    CHECK_EQ(slot.getEndHour(), 10);
    CHECK_EQ(slot.getEndMinute(), 45);
    CHECK_EQ(slot.getStartTimeString(), std::string("09:30"));
    CHECK_EQ(slot.getEndTimeString(), std::string("10:45"));
}

TEST_CASE("TimeSlot rejects invalid ranges") {
    bool threw = false;
    try {
        TimeSlot invalid = TimeSlot::fromMinutes(1, 12 * 60, 10 * 60);
        (void)invalid;
    } catch (...) {
        threw = true;
    }

    CHECK(threw);
}
