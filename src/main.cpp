#include <iostream>
#include <vector>
#include "models/Request.h"
#include "models/OneTimeRequest.h"
#include "models/RecurringRequest.h"
#include "models/Allocation.h"
#include "services/AllocationService.h"
#include "data/DataController.h"
#include "utils/ConsolePrinter.h"

int main() {
    DataController dataController;
    SystemData data = dataController.loadAllData(
        "data/users.csv",
        "data/spaces.csv",
        "data/requests.csv"
    );

    if (data.users.empty() || data.spaces.empty() || data.requests.empty()) {
        std::cout << "Error: Failed to load system data.\n";
        dataController.cleanupData(data);
        return 1;
    }

    AllocationService allocationService;

    Allocation existingClassroomAllocation(100, 999, data.spaces[0], TimeSlot(1, 10, 12));
    allocationService.addExistingAllocation(existingClassroomAllocation);

    for (Request* request : data.requests) {
        std::string label = "Request " + std::to_string(request->getId());

        if (OneTimeRequest* oneTime = dynamic_cast<OneTimeRequest*>(request)) {
            bool result = allocationService.processRequest(*oneTime);
            printOneTimeResult(
                label,
                *oneTime,
                result,
                "One-time request loaded from external CSV source"
            );
        }
        else if (RecurringRequest* recurring = dynamic_cast<RecurringRequest*>(request)) {
            bool result = allocationService.processRequest(*recurring);
            printRecurringResult(
                label,
                *recurring,
                result,
                "Recurring request loaded from external CSV source"
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