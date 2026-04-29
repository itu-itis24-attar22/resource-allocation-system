#ifndef DATACONTROLLER_H
#define DATACONTROLLER_H

#include <string>
#include <vector>
#include "../models/User.h"
#include "../models/Space.h"
#include "../models/Request.h"
#include "../models/Allocation.h"

struct SystemData {
    std::vector<User*> users;
    std::vector<Space*> spaces;
    std::vector<Request*> requests;
};

class DataController {
public:
    std::string loadAllocationStrategyName(const std::string& configFile) const;

    SystemData loadAllData(const std::string& usersFile,
                           const std::string& spacesFile,
                           const std::string& requestsFile) const;

    void exportAllocations(const std::string& allocationsFile,
                           const std::vector<Allocation>& allocations) const;

    void exportRequestResults(const std::string& resultsFile,
                              const std::vector<Request*>& requests) const;

    void cleanupData(SystemData& data) const;
};

#endif
