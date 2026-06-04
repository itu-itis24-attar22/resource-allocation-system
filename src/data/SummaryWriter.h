#ifndef SUMMARYWRITER_H
#define SUMMARYWRITER_H

#include <string>
#include <vector>
#include "../models/Allocation.h"
#include "../models/Request.h"

class SummaryWriter {
public:
    static bool writeRequestSummaries(const std::string& filename,
                                      const std::vector<Request*>& requests,
                                      const std::vector<Allocation>& allocations);

    static bool writeAllocationSummaries(const std::string& filename,
                                         const std::vector<Request*>& requests,
                                         const std::vector<Allocation>& allocations);
};

#endif
