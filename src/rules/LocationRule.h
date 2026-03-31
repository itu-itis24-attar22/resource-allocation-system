#ifndef LOCATIONRULE_H
#define LOCATIONRULE_H

#include "../models/OneTimeRequest.h"
#include "../models/RecurringRequest.h"

class LocationRule {
public:
    bool check(const OneTimeRequest& request) const;
    bool check(const RecurringRequest& request) const;
};

#endif