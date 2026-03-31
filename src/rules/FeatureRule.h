#ifndef FEATURERULE_H
#define FEATURERULE_H

#include "../models/OneTimeRequest.h"
#include "../models/RecurringRequest.h"

class FeatureRule {
public:
    bool check(const OneTimeRequest& request) const;
    bool check(const RecurringRequest& request) const;
};

#endif