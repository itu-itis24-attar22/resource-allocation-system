#include "LocationRule.h"

bool LocationRule::check(const OneTimeRequest& request) const {
    if (request.getRequiredBuilding().empty()) {
        return true;
    }
    return request.getRequestedSpace()->getBuilding() == request.getRequiredBuilding();
}

bool LocationRule::check(const RecurringRequest& request) const {
    if (request.getRequiredBuilding().empty()) {
        return true;
    }
    return request.getRequestedSpace()->getBuilding() == request.getRequiredBuilding();
}