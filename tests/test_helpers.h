#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <string>
#include <vector>
#include "../src/models/Allocation.h"
#include "../src/models/Request.h"

inline bool historyContains(const Request& request, const std::string& text) {
    for (const std::string& event : request.getLifecycleHistory()) {
        if (event.find(text) != std::string::npos) {
            return true;
        }
    }
    return false;
}

inline int allocationCountForRequest(const std::vector<Allocation>& allocations,
                                     int requestId) {
    int count = 0;
    for (const Allocation& allocation : allocations) {
        if (allocation.getRequestId() == requestId) {
            count++;
        }
    }
    return count;
}

inline int assignedParticipantsFor(const std::vector<Allocation>& allocations,
                                   int requestId,
                                   int spaceId) {
    for (const Allocation& allocation : allocations) {
        if (allocation.getRequestId() == requestId &&
            allocation.getSpace() &&
            allocation.getSpace()->getId() == spaceId) {
            return allocation.getAssignedParticipants();
        }
    }
    return 0;
}

#endif
