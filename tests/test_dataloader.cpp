#include "test_framework.h"
#include <cstdio>
#include <fstream>
#include <string>
#include "../src/data/DataLoader.h"

namespace {
    void writeFile(const std::string& path, const std::string& contents) {
        std::ofstream file(path);
        file << contents;
    }
}

TEST_CASE("DataLoader loads valid user busy slots and skips malformed rows") {
    const std::string path = "tests/tmp_user_busy_slots.csv";
    writeFile(
        path,
        "userId,day,startTime,endTime,reason\n"
        "2,1,10:00,12:00,Lecture\n"
        "bad,1,10:00,12:00,Invalid user id\n"
        "3,8,09:00,10:00,Invalid day\n"
        "4,2,12:00,11:00,Invalid range\n"
        "5,2,09:00,10:00,Office hour\n"
    );

    std::vector<UserBusySlot> slots = DataLoader::loadUserBusySlots(path);
    std::remove(path.c_str());

    CHECK_EQ(slots.size(), static_cast<size_t>(2));
    CHECK_EQ(slots[0].getUserId(), 2);
    CHECK_EQ(slots[0].getTimeSlot().getStartTimeString(), std::string("10:00"));
    CHECK_EQ(slots[0].getTimeSlot().getEndTimeString(), std::string("12:00"));
    CHECK_EQ(slots[1].getUserId(), 5);
}

TEST_CASE("DataLoader handles missing optional user busy slot file") {
    std::vector<UserBusySlot> slots =
        DataLoader::loadUserBusySlots("tests/does_not_exist_user_busy_slots.csv");

    CHECK(slots.empty());
}

TEST_CASE("DataLoader loads valid request participants and skips malformed rows") {
    const std::string path = "tests/tmp_request_participants.csv";
    writeFile(
        path,
        "requestId,userId,participantRole\n"
        "40,2,Supervisor\n"
        "40,4,CommitteeMember\n"
        "bad,5,Participant\n"
        "41,missing,Participant\n"
        "42,6,\n"
    );

    std::vector<RequestParticipant> participants =
        DataLoader::loadRequestParticipants(path);
    std::remove(path.c_str());

    CHECK_EQ(participants.size(), static_cast<size_t>(2));
    CHECK_EQ(participants[0].getRequestId(), 40);
    CHECK_EQ(participants[0].getUserId(), 2);
    CHECK_EQ(participants[0].getParticipantRole(), std::string("Supervisor"));
    CHECK_EQ(participants[1].getParticipantRole(), std::string("CommitteeMember"));
}

TEST_CASE("DataLoader handles missing optional request participants file") {
    std::vector<RequestParticipant> participants =
        DataLoader::loadRequestParticipants("tests/does_not_exist_request_participants.csv");

    CHECK(participants.empty());
}
