#include "test_framework.h"
#include <cstdio>
#include <fstream>
#include <vector>
#include "../src/data/AllocationWriter.h"
#include "../src/data/RequestResultWriter.h"
#include "../src/data/SummaryWriter.h"
#include "../src/models/Allocation.h"
#include "../src/models/Classroom.h"
#include "../src/models/OneTimeRequest.h"
#include "../src/models/Student.h"
#include "../src/rules/AvailabilityRule.h"
#include "../src/rules/CapacityRule.h"
#include "../src/rules/FeatureRule.h"
#include "../src/rules/LocationRule.h"
#include "../src/rules/RuleEngineFacade.h"
#include "../src/rules/StatusRule.h"
#include "../src/rules/UserRoleRule.h"
#include "../src/strategies/PriorityAllocationStrategy.h"

namespace {
    bool fileExistsAndNotEmpty(const std::string& path) {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        return file.is_open() && file.tellg() > 0;
    }
}

TEST_CASE("Output writers tolerate null pointers without crashing") {
    const std::string allocationsPath = "tests/tmp_null_allocations.csv";
    const std::string resultsPath = "tests/tmp_null_results.csv";
    const std::string requestSummariesPath = "tests/tmp_null_request_summaries.csv";
    const std::string allocationSummariesPath = "tests/tmp_null_allocation_summaries.csv";

    TimeSlot slot(1, 9, 10);
    Allocation missingSpaceAllocation(1, 44, nullptr, slot, 3);
    std::vector<Allocation> allocations{missingSpaceAllocation};
    std::vector<Request*> requests{nullptr};

    CHECK(AllocationWriter::writeAllocations(allocationsPath, allocations));
    CHECK(RequestResultWriter::writeRequestResults(resultsPath, requests));
    CHECK(SummaryWriter::writeRequestSummaries(requestSummariesPath, requests, allocations));
    CHECK(SummaryWriter::writeAllocationSummaries(allocationSummariesPath, requests, allocations));

    CHECK(fileExistsAndNotEmpty(allocationsPath));
    CHECK(fileExistsAndNotEmpty(resultsPath));
    CHECK(fileExistsAndNotEmpty(requestSummariesPath));
    CHECK(fileExistsAndNotEmpty(allocationSummariesPath));
    CHECK_FILE_CONTAINS(allocationsPath, "1,44,0");
    CHECK_FILE_CONTAINS(allocationSummariesPath, "44");

    std::remove(allocationsPath.c_str());
    std::remove(resultsPath.c_str());
    std::remove(requestSummariesPath.c_str());
    std::remove(allocationSummariesPath.c_str());
}

TEST_CASE("Output writers fail safely when export files cannot be opened") {
    TimeSlot slot(1, 9, 10);
    Allocation allocation(1, 44, nullptr, slot, 3);
    std::vector<Allocation> allocations{allocation};
    std::vector<Request*> requests{nullptr};

    CHECK(!AllocationWriter::writeAllocations(
        "tests/missing_export_dir/allocations.csv",
        allocations
    ));
    CHECK(!RequestResultWriter::writeRequestResults(
        "tests/missing_export_dir/results.csv",
        requests
    ));
    CHECK(!SummaryWriter::writeRequestSummaries(
        "tests/missing_export_dir/request_summaries.csv",
        requests,
        allocations
    ));
    CHECK(!SummaryWriter::writeAllocationSummaries(
        "tests/missing_export_dir/allocation_summaries.csv",
        requests,
        allocations
    ));

    CHECK(!fileExistsAndNotEmpty("tests/missing_export_dir/allocations.csv"));
    CHECK(!fileExistsAndNotEmpty("tests/missing_export_dir/results.csv"));
}

TEST_CASE("Rules reject requests with missing requested space safely") {
    Student student(1, "Student");
    OneTimeRequest missingSpace(1, &student, nullptr, TimeSlot(1, 9, 10),
                                5, "Missing Space", "Meeting", "Projector", "Engineering");

    CHECK(!CapacityRule().evaluate(missingSpace).isPassed());
    CHECK(!FeatureRule().evaluate(missingSpace).isPassed());
    CHECK(!StatusRule().evaluate(missingSpace).isPassed());
    CHECK(!UserRoleRule().evaluate(missingSpace).isPassed());
    CHECK(!AvailabilityRule().check(missingSpace, std::vector<Allocation>{}));

    RuleEvaluationResult locationResult = LocationRule().evaluate(missingSpace);
    CHECK(!locationResult.isPassed());
    CHECK_CONTAINS(locationResult.getFailureReason(), "Requested space missing");
}

TEST_CASE("Availability helpers ignore corrupt allocations with missing spaces") {
    Student student(1, "Student");
    Classroom room(101, "B201", 40, true, true, false, true, "Engineering");
    OneTimeRequest request(1, &student, &room, TimeSlot(1, 9, 10),
                           5, "Safe Availability", "Meeting", "", "");
    std::vector<Allocation> allocations{
        Allocation(1, 999, nullptr, TimeSlot(1, 9, 10), 5)
    };

    CHECK(AvailabilityRule().check(request, allocations));
}

TEST_CASE("Priority strategy skips null request entries safely") {
    Student student(1, "Student");
    Classroom room(101, "B201", 40, true, true, false, true, "Engineering");
    OneTimeRequest request(1, &student, &room, TimeSlot(1, 9, 10),
                           5, "Priority Null Safety", "Meeting", "", "");
    std::vector<Request*> requests{nullptr, &request};
    std::vector<Allocation> allocations;
    std::vector<Space*> spaces{&room};
    RuleEngineFacade facade;

    PriorityAllocationStrategy strategy;
    strategy.processRequests(requests, spaces, allocations, facade);

    CHECK(request.getStatus() == RequestStatus::Approved);
    CHECK_EQ(allocations.size(), static_cast<size_t>(1));
}
