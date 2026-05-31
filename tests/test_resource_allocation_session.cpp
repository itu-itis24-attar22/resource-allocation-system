#include "test_framework.h"
#include "../src/services/ResourceAllocationSession.h"
#include <cstdio>
#include <fstream>

namespace {
    void writeSessionCsv(const std::string& path, const std::string& contents) {
        std::ofstream file(path);
        file << contents;
    }

    bool sessionFileExistsAndNotEmpty(const std::string& path) {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        return file.is_open() && file.tellg() > 0;
    }
}

TEST_CASE("ResourceAllocationSession loads, runs, exports, and cleans up backend workflow") {
    const std::string configPath = "tests/tmp_session_config.txt";
    const std::string usersPath = "tests/tmp_session_users.csv";
    const std::string spacesPath = "tests/tmp_session_spaces.csv";
    const std::string requestsPath = "tests/tmp_session_requests.csv";
    const std::string busySlotsPath = "tests/tmp_session_busy_slots.csv";
    const std::string participantsPath = "tests/tmp_session_participants.csv";
    const std::string allocationsPath = "tests/tmp_session_allocations.csv";
    const std::string resultsPath = "tests/tmp_session_results.csv";
    const std::string requestSummariesPath = "tests/tmp_session_request_summaries.csv";
    const std::string allocationSummariesPath = "tests/tmp_session_allocation_summaries.csv";

    writeSessionCsv(configPath, "allocation_strategy=priority\n");
    writeSessionCsv(
        usersPath,
        "userId,name,role\n"
        "1,Student One,Student\n"
    );
    writeSessionCsv(
        spacesPath,
        "spaceId,type,name,capacity,hasProjector,hasWhiteboard,hasComputers,isAvailable,building\n"
        "301,MeetingRoom,M301,10,1,0,0,1,AdminBuilding\n"
    );
    writeSessionCsv(
        requestsPath,
        "requestId,requestType,userId,spaceId,participantCount,requiredFeature,requiredBuilding,timeData,title,purpose,courseCode,courseName,examType,canSplitAcrossRooms\n"
        "90,OneTime,1,301,5,Projector,AdminBuilding,1-09:00-10:00,Facade Request,Facade workflow test,,,,false\n"
    );
    writeSessionCsv(busySlotsPath, "userId,day,startTime,endTime,reason\n");
    writeSessionCsv(participantsPath, "requestId,userId,participantRole\n");

    ResourceAllocationSession session;
    CHECK_EQ(session.loadStrategyName(configPath), std::string("priority"));
    CHECK(session.loadSystemData(
        usersPath,
        spacesPath,
        requestsPath,
        busySlotsPath,
        participantsPath
    ));
    CHECK(session.isDataLoaded());
    CHECK_EQ(session.getSystemData().requests.size(), static_cast<size_t>(1));

    CHECK(session.runAllocation());
    CHECK(session.hasRunAllocation());
    CHECK_EQ(session.getAllocations().size(), static_cast<size_t>(1));
    CHECK(session.getSystemData().requests[0]->getStatus() == RequestStatus::Approved);

    session.exportResults(
        allocationsPath,
        resultsPath,
        requestSummariesPath,
        allocationSummariesPath
    );
    CHECK(sessionFileExistsAndNotEmpty(allocationsPath));
    CHECK(sessionFileExistsAndNotEmpty(resultsPath));
    CHECK(sessionFileExistsAndNotEmpty(requestSummariesPath));
    CHECK(sessionFileExistsAndNotEmpty(allocationSummariesPath));
    CHECK_FILE_CONTAINS(resultsPath, "Facade Request");
    CHECK_FILE_CONTAINS(resultsPath, "Approved");
    CHECK_FILE_CONTAINS(requestSummariesPath, "Facade Request");
    CHECK_FILE_CONTAINS(requestSummariesPath, "Facade workflow test");
    CHECK_FILE_CONTAINS(requestSummariesPath, "Approved");
    CHECK_FILE_CONTAINS(requestSummariesPath, "90");
    CHECK_FILE_CONTAINS(allocationSummariesPath, "Facade Request");
    CHECK_FILE_CONTAINS(allocationSummariesPath, "assignedParticipants");
    CHECK_FILE_CONTAINS(allocationSummariesPath, "5");

    session.cleanup();
    CHECK(!session.isDataLoaded());
    CHECK(session.getSystemData().requests.empty());
    CHECK(session.getAllocations().empty());

    std::remove(configPath.c_str());
    std::remove(usersPath.c_str());
    std::remove(spacesPath.c_str());
    std::remove(requestsPath.c_str());
    std::remove(busySlotsPath.c_str());
    std::remove(participantsPath.c_str());
    std::remove(allocationsPath.c_str());
    std::remove(resultsPath.c_str());
    std::remove(requestSummariesPath.c_str());
    std::remove(allocationSummariesPath.c_str());
}

TEST_CASE("ResourceAllocationSession reports failed loads without running allocation") {
    const std::string usersPath = "tests/tmp_session_missing_users.csv";
    const std::string spacesPath = "tests/tmp_session_missing_spaces.csv";

    writeSessionCsv(
        usersPath,
        "userId,name,role\n"
        "1,Student One,Student\n"
    );
    writeSessionCsv(
        spacesPath,
        "spaceId,type,name,capacity,hasProjector,hasWhiteboard,hasComputers,isAvailable,building\n"
        "301,MeetingRoom,M301,10,1,0,0,1,AdminBuilding\n"
    );

    ResourceAllocationSession session;
    CHECK(!session.loadSystemData(
        usersPath,
        spacesPath,
        "tests/tmp_session_missing_requests.csv"
    ));
    CHECK(!session.isDataLoaded());
    CHECK(!session.runAllocation());
    CHECK(!session.hasRunAllocation());
    CHECK(session.getAllocations().empty());

    session.cleanup();
    std::remove(usersPath.c_str());
    std::remove(spacesPath.c_str());
}
