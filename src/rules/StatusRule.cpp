#include "StatusRule.h"

bool StatusRule::check(const Request& request) const {
    return request.getRequestedSpace()->getIsAvailable();
}