#include "test_framework.h"
#include "test_helpers.h"
#include "../src/data/DataController.h"
#include "../src/models/InvalidRequest.h"
#include "../src/services/AllocationService.h"
#include <cstdio>
#include <fstream>

namespace {
    void writeCsv(const std::string& path, const std::string& contents) {
        std::ofstream file(path);
        file << contents;
    }

    bool fileExistsAndNotEmpty(const std::string& path) {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        return file.is_open() && file.tellg() > 0;
    }
}

TEST_CASE("DataController loads temp CSVs, processes requests, and exports results") {
    const std::string usersPath = "tests/tmp_users.csv";
    const std::string spacesPath = "tests/tmp_spaces.csv";
    const std::string requestsPath = "tests/tmp_requests.csv";
    const std::string allocationsPath = "tests/tmp_allocations.csv";
    const std::string resultsPath = "tests/tmp_request_results.csv";

    writeCsv(
        usersPath,
        "userId,name,role\n"
        "1,Student One,Student\n"
        "2,Dr Priority,Instructor\n"
    );
    writeCsv(
        spacesPath,
        "spaceId,type,name,capacity,hasProjector,hasWhiteboard,hasComputers,isAvailable,building\n"
        "301,MeetingRoom,M301,10,1,0,0,1,AdminBuilding\n"
    );
    writeCsv(
        requestsPath,
        "requestId,requestType,userId,spaceId,participantCount,requiredFeature,requiredBuilding,timeData,title,purpose,courseCode,courseName,examType,canSplitAcrossRooms\n"
        "1,OneTime,1,301,5,Projector,AdminBuilding,1-10-11,Student Meeting,Meeting,,,,false\n"
        "2,OneTime,2,301,5,Projector,AdminBuilding,1-10-11,Instructor Conflict,Meeting,,,,false\n"
    );

    DataController controller;
    SystemData data = controller.loadAllData(usersPath, spacesPath, requestsPath);

    CHECK_EQ(data.users.size(), static_cast<size_t>(2));
    CHECK_EQ(data.spaces.size(), static_cast<size_t>(1));
    CHECK_EQ(data.requests.size(), static_cast<size_t>(2));

    AllocationService service("priority", data.users, data.userBusySlots);
    service.processRequests(data.requests, data.spaces);
    controller.exportAllocations(allocationsPath, service.getAllocations());
    controller.exportRequestResults(resultsPath, data.requests);

    CHECK(data.requests[0]->getStatus() == RequestStatus::Rejected);
    CHECK(data.requests[1]->getStatus() == RequestStatus::Approved);
    CHECK(fileExistsAndNotEmpty(allocationsPath));
    CHECK(fileExistsAndNotEmpty(resultsPath));
    CHECK_FILE_CONTAINS(resultsPath, "Student Meeting");
    CHECK_FILE_CONTAINS(resultsPath, "Instructor Conflict");
    CHECK_FILE_CONTAINS(resultsPath, "Instructor");
    CHECK_FILE_CONTAINS(resultsPath, "Rejected");
    CHECK_FILE_CONTAINS(resultsPath, "Approved");
    CHECK_FILE_CONTAINS(resultsPath, "Time slot unavailable");
    CHECK_FILE_CONTAINS(resultsPath, "exported");

    controller.cleanupData(data);
    std::remove(usersPath.c_str());
    std::remove(spacesPath.c_str());
    std::remove(requestsPath.c_str());
    std::remove(allocationsPath.c_str());
    std::remove(resultsPath.c_str());
}

TEST_CASE("DataController converts invalid request rows into InvalidRequest objects") {
    const std::string usersPath = "tests/tmp_invalid_users.csv";
    const std::string spacesPath = "tests/tmp_invalid_spaces.csv";
    const std::string requestsPath = "tests/tmp_invalid_requests.csv";

    writeCsv(
        usersPath,
        "userId,name,role\n"
        "1,Student One,Student\n"
    );
    writeCsv(
        spacesPath,
        "spaceId,type,name,capacity,hasProjector,hasWhiteboard,hasComputers,isAvailable,building\n"
        "301,MeetingRoom,M301,10,1,0,0,1,AdminBuilding\n"
    );
    writeCsv(
        requestsPath,
        "requestId,requestType,userId,spaceId,participantCount,requiredFeature,requiredBuilding,timeData\n"
        "1,OneTime,1,301,5,Projector,AdminBuilding,1-10-11\n"
        "2,OneTime,99,301,5,Projector,AdminBuilding,1-10-11\n"
        "3,OneTime,1,999,5,Projector,AdminBuilding,1-10-11\n"
        "4,OneTime,1,301,5,Projector,AdminBuilding,bad-time\n"
        "5,UnknownType,1,301,5,Projector,AdminBuilding,1-10-11\n"
    );

    DataController controller;
    SystemData data = controller.loadAllData(usersPath, spacesPath, requestsPath);

    CHECK_EQ(data.requests.size(), static_cast<size_t>(5));
    CHECK(dynamic_cast<InvalidRequest*>(data.requests[0]) == nullptr);
    CHECK(dynamic_cast<InvalidRequest*>(data.requests[1]) != nullptr);
    CHECK(dynamic_cast<InvalidRequest*>(data.requests[2]) != nullptr);
    CHECK(dynamic_cast<InvalidRequest*>(data.requests[3]) != nullptr);
    CHECK(dynamic_cast<InvalidRequest*>(data.requests[4]) != nullptr);
    CHECK_EQ(data.requests[1]->getRejectionReason(), std::string("Invalid user reference"));
    CHECK_EQ(data.requests[2]->getRejectionReason(), std::string("Invalid space reference"));
    CHECK_EQ(data.requests[3]->getRejectionReason(), std::string("Malformed input"));

    CHECK_EQ(data.requests[0]->getTitle(), std::string("Untitled Request"));
    CHECK_EQ(data.requests[0]->getPurpose(), std::string("General"));

    controller.cleanupData(data);
    std::remove(usersPath.c_str());
    std::remove(spacesPath.c_str());
    std::remove(requestsPath.c_str());
}

TEST_CASE("Backend smoke artifacts from main data exist after backend run") {
    const bool allocationsReady = fileExistsAndNotEmpty("data/allocations.csv");
    const bool resultsReady = fileExistsAndNotEmpty("data/request_results.csv");

    if (!allocationsReady || !resultsReady) {
        std::cout << "[INFO] Backend smoke artifacts are missing. "
                  << "Run the normal backend executable to create them.\n";
    }

    CHECK(allocationsReady);
    CHECK(resultsReady);
}
