#include "AllocationWriter.h"
#include <fstream>
#include <iostream>

namespace {
    std::string escapeCsv(const std::string& value) {
        bool needsQuotes = false;
        std::string escaped;

        for (char ch : value) {
            if (ch == '"' || ch == ',' || ch == '\n' || ch == '\r') {
                needsQuotes = true;
            }

            if (ch == '"') {
                escaped += "\"\"";
            } else {
                escaped += ch;
            }
        }

        if (!needsQuotes) {
            return escaped;
        }

        return "\"" + escaped + "\"";
    }
}

void AllocationWriter::writeAllocations(const std::string& filename,
                                        const std::vector<Allocation>& allocations) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open allocation output file: " << filename << "\n";
        return;
    }

    file << "allocationId,requestId,spaceId,spaceName,spaceType,building,day,startHour,endHour,assignedParticipants\n";

    for (const Allocation& allocation : allocations) {
        file << allocation.getId() << ","
             << allocation.getRequestId() << ","
             << allocation.getSpace()->getId() << ","
             << escapeCsv(allocation.getSpace()->getName()) << ","
             << escapeCsv(allocation.getSpace()->getType()) << ","
             << escapeCsv(allocation.getSpace()->getBuilding()) << ","
             << allocation.getTimeSlot().getDay() << ","
             << allocation.getTimeSlot().getStartTimeString() << ","
             << allocation.getTimeSlot().getEndTimeString() << ","
             << allocation.getAssignedParticipants() << "\n";
    }

    file.close();
}
