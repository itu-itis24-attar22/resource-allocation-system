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

    int spaceIdOrDefault(const Allocation& allocation) {
        return allocation.getSpace() ? allocation.getSpace()->getId() : 0;
    }

    std::string spaceNameOrDefault(const Allocation& allocation) {
        return allocation.getSpace() ? allocation.getSpace()->getName() : "";
    }

    std::string spaceTypeOrDefault(const Allocation& allocation) {
        return allocation.getSpace() ? allocation.getSpace()->getType() : "";
    }

    std::string buildingOrDefault(const Allocation& allocation) {
        return allocation.getSpace() ? allocation.getSpace()->getBuilding() : "";
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
        if (!allocation.getSpace()) {
            std::cerr << "Warning: Allocation " << allocation.getId()
                      << " has no space. Writing empty space fields.\n";
        }

        file << allocation.getId() << ","
             << allocation.getRequestId() << ","
             << spaceIdOrDefault(allocation) << ","
             << escapeCsv(spaceNameOrDefault(allocation)) << ","
             << escapeCsv(spaceTypeOrDefault(allocation)) << ","
             << escapeCsv(buildingOrDefault(allocation)) << ","
             << allocation.getTimeSlot().getDay() << ","
             << allocation.getTimeSlot().getStartTimeString() << ","
             << allocation.getTimeSlot().getEndTimeString() << ","
             << allocation.getAssignedParticipants() << "\n";
    }

    file.close();
}
