#include "CapacityRule.h"

bool CapacityRule::check(const Request& request) const {
    return request.getRequestedSpace()->getCapacity() >= request.getParticipantCount();
}