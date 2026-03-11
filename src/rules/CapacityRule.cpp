#include "CapacityRule.h"

bool CapacityRule::check(const OneTimeRequest& request) const {
    return request.getRequestedSpace()->getCapacity() >= request.getParticipantCount();
}