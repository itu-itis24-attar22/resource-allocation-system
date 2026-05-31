#include "test_framework.h"
#include "test_helpers.h"
#include "../src/data/DataController.h"
#include "../src/models/Administrator.h"
#include "../src/models/Instructor.h"
#include "../src/models/MeetingRoom.h"
#include "../src/models/OneTimeRequest.h"
#include "../src/models/Student.h"
#include "../src/services/AllocationService.h"
#include "../src/strategies/AllocationStrategyFactory.h"
#include "../src/strategies/GreedyAllocationStrategy.h"
#include "../src/strategies/PriorityAllocationStrategy.h"
#include <cstdio>
#include <fstream>

namespace {
    void writeTextFile(const std::string& path, const std::string& contents) {
        std::ofstream file(path);
        file << contents;
    }
}

TEST_CASE("AllocationStrategyFactory returns expected strategies and greedy fallback") {
    const AllocationStrategyFactory& factory = AllocationStrategyFactory::getInstance();

    CHECK(dynamic_cast<const GreedyAllocationStrategy*>(factory.getStrategy("greedy")) != nullptr);
    CHECK(dynamic_cast<const PriorityAllocationStrategy*>(factory.getStrategy("priority")) != nullptr);
    CHECK(dynamic_cast<const GreedyAllocationStrategy*>(factory.getStrategy("")) != nullptr);
    CHECK(dynamic_cast<const GreedyAllocationStrategy*>(factory.getStrategy("unknown")) != nullptr);
    CHECK(dynamic_cast<const PriorityAllocationStrategy*>(factory.getStrategy("PRIORITY")) != nullptr);
}

TEST_CASE("DataController loads allocation strategy from config with safe fallbacks") {
    DataController controller;
    const std::string priorityConfig = "tests/tmp_priority_config.txt";
    const std::string greedyConfig = "tests/tmp_greedy_config.txt";
    const std::string emptyConfig = "tests/tmp_empty_config.txt";
    const std::string invalidConfig = "tests/tmp_invalid_config.txt";

    writeTextFile(priorityConfig, "allocation_strategy=priority\n");
    writeTextFile(greedyConfig, "allocation_strategy=greedy\n");
    writeTextFile(emptyConfig, "# no strategy here\n");
    writeTextFile(invalidConfig, "allocation_strategy=does_not_exist\n");

    CHECK_EQ(controller.loadAllocationStrategyName(priorityConfig), std::string("priority"));
    CHECK_EQ(controller.loadAllocationStrategyName(greedyConfig), std::string("greedy"));
    CHECK_EQ(controller.loadAllocationStrategyName(emptyConfig), std::string("greedy"));
    CHECK_EQ(controller.loadAllocationStrategyName(invalidConfig), std::string("greedy"));
    CHECK_EQ(controller.loadAllocationStrategyName("tests/missing_config.txt"), std::string("greedy"));

    std::remove(priorityConfig.c_str());
    std::remove(greedyConfig.c_str());
    std::remove(emptyConfig.c_str());
    std::remove(invalidConfig.c_str());
}

TEST_CASE("Greedy strategy processes conflicting requests in input order") {
    Student student(1, "Student");
    Administrator administrator(2, "Admin");
    MeetingRoom room(301, "M301", 10, true, false, false, true, "AdminBuilding");
    std::vector<Request*> requests;
    OneTimeRequest studentRequest(1, &student, &room, TimeSlot(1, 10, 11),
                                  5, "Student", "Meeting", "Projector", "AdminBuilding");
    OneTimeRequest adminRequest(2, &administrator, &room, TimeSlot(1, 10, 11),
                                5, "Admin", "Meeting", "Projector", "AdminBuilding");
    requests.push_back(&studentRequest);
    requests.push_back(&adminRequest);

    AllocationService service("greedy");
    service.processRequests(requests);

    CHECK(studentRequest.getStatus() == RequestStatus::Approved);
    CHECK(adminRequest.getStatus() == RequestStatus::Rejected);
    CHECK_EQ(service.getAllocations()[0].getRequestId(), 1);
}

TEST_CASE("Priority strategy processes conflicting requests by descending priority") {
    Student student(1, "Student");
    Administrator administrator(2, "Admin");
    MeetingRoom room(301, "M301", 10, true, false, false, true, "AdminBuilding");
    std::vector<Request*> requests;
    OneTimeRequest studentRequest(1, &student, &room, TimeSlot(1, 10, 11),
                                  5, "Student", "Meeting", "Projector", "AdminBuilding");
    OneTimeRequest adminRequest(2, &administrator, &room, TimeSlot(1, 10, 11),
                                5, "Admin", "Meeting", "Projector", "AdminBuilding");
    requests.push_back(&studentRequest);
    requests.push_back(&adminRequest);

    AllocationService service("priority");
    service.processRequests(requests);

    CHECK(studentRequest.getStatus() == RequestStatus::Rejected);
    CHECK(adminRequest.getStatus() == RequestStatus::Approved);
    CHECK_EQ(service.getAllocations()[0].getRequestId(), 2);
}

TEST_CASE("Priority strategy preserves input order for equal priority conflicts") {
    Instructor firstInstructor(1, "Dr. First");
    Instructor secondInstructor(2, "Dr. Second");
    MeetingRoom room(301, "M301", 10, true, false, false, true, "AdminBuilding");
    std::vector<Request*> requests;
    OneTimeRequest firstRequest(1, &firstInstructor, &room, TimeSlot(1, 10, 11),
                                5, "First", "Meeting", "Projector", "AdminBuilding");
    OneTimeRequest secondRequest(2, &secondInstructor, &room, TimeSlot(1, 10, 11),
                                 5, "Second", "Meeting", "Projector", "AdminBuilding");
    requests.push_back(&firstRequest);
    requests.push_back(&secondRequest);

    AllocationService service("priority");
    service.processRequests(requests);

    CHECK(firstRequest.getStatus() == RequestStatus::Approved);
    CHECK(secondRequest.getStatus() == RequestStatus::Rejected);
    CHECK_EQ(service.getAllocations()[0].getRequestId(), 1);
}
