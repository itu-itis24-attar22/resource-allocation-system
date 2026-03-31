#include "StatusRule.h"

bool StatusRule::check(const OneTimeRequest& request) const {
    return request.getRequestedSpace()->getIsAvailable();
}

bool StatusRule::check(const RecurringRequest& request) const {
    return request.getRequestedSpace()->getIsAvailable();
}