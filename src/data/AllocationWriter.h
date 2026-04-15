#ifndef ALLOCATIONWRITER_H
#define ALLOCATIONWRITER_H

#include <string>
#include <vector>
#include "../models/Allocation.h"

class AllocationWriter {
public:
    static void writeAllocations(const std::string& filename,
                                 const std::vector<Allocation>& allocations);
};

#endif