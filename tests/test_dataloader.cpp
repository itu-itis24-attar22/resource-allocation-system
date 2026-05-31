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

TEST_CASE("DataLoader handles missing and empty required CSV files safely") {
    const std::string emptyUsers = "tests/tmp_empty_users.csv";
    const std::string emptySpaces = "tests/tmp_empty_spaces.csv";
    writeFile(emptyUsers, "");
    writeFile(emptySpaces, "");

    std::vector<User*> missingUsers =
        DataLoader::loadUsers("tests/does_not_exist_users.csv");
    std::vector<Space*> missingSpaces =
        DataLoader::loadSpaces("tests/does_not_exist_spaces.csv");
    std::vector<User*> emptyLoadedUsers = DataLoader::loadUsers(emptyUsers);
    std::vector<Space*> emptyLoadedSpaces = DataLoader::loadSpaces(emptySpaces);

    CHECK(missingUsers.empty());
    CHECK(missingSpaces.empty());
    CHECK(emptyLoadedUsers.empty());
    CHECK(emptyLoadedSpaces.empty());

    std::remove(emptyUsers.c_str());
    std::remove(emptySpaces.c_str());
}

TEST_CASE("DataLoader skips unknown user roles and unknown space types") {
    const std::string usersPath = "tests/tmp_unknown_roles.csv";
    const std::string spacesPath = "tests/tmp_unknown_spaces.csv";
    writeFile(
        usersPath,
        "userId,name,role\n"
        "1,Alice,Student\n"
        "2,Bob,Dean\n"
        "3,Carol,TeachingAssistant\n"
    );
    writeFile(
        spacesPath,
        "spaceId,type,name,capacity,hasProjector,hasWhiteboard,hasComputers,isAvailable,building\n"
        "101,Classroom,B201,40,1,1,0,1,Engineering\n"
        "999,Auditorium,A1,100,1,1,0,1,Main\n"
    );

    std::vector<User*> users = DataLoader::loadUsers(usersPath);
    std::vector<Space*> spaces = DataLoader::loadSpaces(spacesPath);

    CHECK_EQ(users.size(), static_cast<size_t>(2));
    CHECK_EQ(users[0]->getRoleName(), std::string("Student"));
    CHECK_EQ(users[1]->getRoleName(), std::string("TeachingAssistant"));
    CHECK_EQ(spaces.size(), static_cast<size_t>(1));
    CHECK_EQ(spaces[0]->getType(), std::string("Classroom"));

    for (User* user : users) {
        delete user;
    }
    for (Space* space : spaces) {
        delete space;
    }
    std::remove(usersPath.c_str());
    std::remove(spacesPath.c_str());
}

TEST_CASE("DataLoader currently allows duplicate user and space IDs") {
    const std::string usersPath = "tests/tmp_duplicate_users.csv";
    const std::string spacesPath = "tests/tmp_duplicate_spaces.csv";
    writeFile(
        usersPath,
        "userId,name,role\n"
        "1,Alice,Student\n"
        "1,Duplicate Alice,Student\n"
    );
    writeFile(
        spacesPath,
        "spaceId,type,name,capacity,hasProjector,hasWhiteboard,hasComputers,isAvailable,building\n"
        "101,Classroom,B201,40,1,1,0,1,Engineering\n"
        "101,Classroom,B201 Duplicate,30,1,1,0,1,Engineering\n"
    );

    std::vector<User*> users = DataLoader::loadUsers(usersPath);
    std::vector<Space*> spaces = DataLoader::loadSpaces(spacesPath);

    CHECK_EQ(users.size(), static_cast<size_t>(2));
    CHECK_EQ(spaces.size(), static_cast<size_t>(2));

    for (User* user : users) {
        delete user;
    }
    for (Space* space : spaces) {
        delete space;
    }
    std::remove(usersPath.c_str());
    std::remove(spacesPath.c_str());
}
