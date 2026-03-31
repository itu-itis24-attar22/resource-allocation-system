#include "FeatureRule.h"

bool FeatureRule::check(const OneTimeRequest& request) const {
    return request.getRequestedSpace()->hasFeature(request.getRequiredFeature());
}

bool FeatureRule::check(const RecurringRequest& request) const {
    return request.getRequestedSpace()->hasFeature(request.getRequiredFeature());
}