#include "AllocationWriter.h"
#include <fstream>
#include <iostream>

void AllocationWriter::writeAllocations(const std::string& filename,
                                        const std::vector<Allocation>& allocations) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open allocation output file: " << filename << "\n";
        return;
    }

    file << "allocationId,requestId,spaceId,spaceName,spaceType,building,day,startHour,endHour\n";

    for (const Allocation& allocation : allocations) {
        file << allocation.getId() << ","
             << allocation.getRequestId() << ","
             << allocation.getSpace()->getId() << ","
             << allocation.getSpace()->getName() << ","
             << allocation.getSpace()->getType() << ","
             << allocation.getSpace()->getBuilding() << ","
             << allocation.getTimeSlot().getDay() << ","
             << allocation.getTimeSlot().getStartHour() << ","
             << allocation.getTimeSlot().getEndHour() << "\n";
    }

    file.close();
}