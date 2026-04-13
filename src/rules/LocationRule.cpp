#include "LocationRule.h"

bool LocationRule::check(const Request& request) const {
    if (request.getRequiredBuilding().empty()) {
        return true;
    }
    return request.getRequestedSpace()->getBuilding() == request.getRequiredBuilding();
}