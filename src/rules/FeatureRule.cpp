#include "FeatureRule.h"

bool FeatureRule::check(const Request& request) const {
    return request.getRequestedSpace()->hasFeature(request.getRequiredFeature());
}