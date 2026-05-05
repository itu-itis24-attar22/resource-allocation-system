#include <iostream>
#include <vector>
#include <cstdio>
#include "models/Request.h"
#include "models/OneTimeRequest.h"
#include "models/RecurringRequest.h"
#include "models/ExamRequest.h"
#include "models/InvalidRequest.h"
#include "models/Allocation.h"
#include "services/AllocationService.h"
#include "data/DataController.h"
#include "utils/ConsolePrinter.h"

int main() {
    DataController dataController;
    std::string strategyName = dataController.loadAllocationStrategyName("data/config.txt");
    SystemData data = dataController.loadAllData(
        "data/users.csv",
        "data/spaces.csv",
        "data/requests.csv"
    );

    if (data.users.empty() || data.spaces.empty() || data.requests.empty()) {
        std::remove("data/allocations.csv");
        std::remove("data/request_results.csv");
        std::cout << "Error: Failed to load system data.\n";
        std::cout << "Stale output files were cleared.\n";
        dataController.cleanupData(data);
        return 1;
    }

    AllocationService allocationService(strategyName);

    Allocation existingClassroomAllocation(100, 999, data.spaces[0], TimeSlot(1, 10, 12),
                                           data.spaces[0]->getCapacity());
    allocationService.addExistingAllocation(existingClassroomAllocation);

    allocationService.processRequests(data.requests);

    for (Request* request : data.requests) {
        std::string label = "Request " + std::to_string(request->getId());

        if (OneTimeRequest* oneTime = dynamic_cast<OneTimeRequest*>(request)) {
            bool result = oneTime->getStatus() == RequestStatus::Approved;
            printOneTimeResult(
                label,
                *oneTime,
                result,
                "One-time request loaded from external CSV source"
            );
        }
        else if (RecurringRequest* recurring = dynamic_cast<RecurringRequest*>(request)) {
            bool result = recurring->getStatus() == RequestStatus::Approved;
            printRecurringResult(
                label,
                *recurring,
                result,
                "Recurring request loaded from external CSV source"
            );
        }
        else if (ExamRequest* exam = dynamic_cast<ExamRequest*>(request)) {
            bool result = exam->getStatus() == RequestStatus::Approved;
            printExamResult(
                label,
                *exam,
                result,
                "Exam request loaded from external CSV source"
            );
        }
        else if (InvalidRequest* invalid = dynamic_cast<InvalidRequest*>(request)) {
            printInvalidResult(
                label,
                *invalid,
                "Invalid request loaded from external CSV source"
            );
        }
    }

    allocationService.printAllocations();

    dataController.exportAllocations("data/allocations.csv", allocationService.getAllocations());
    std::cout << "\nAllocations exported to data/allocations.csv\n";

    dataController.exportRequestResults("data/request_results.csv", data.requests);
    std::cout << "Request results exported to data/request_results.csv\n";

    dataController.cleanupData(data);
    return 0;
}
