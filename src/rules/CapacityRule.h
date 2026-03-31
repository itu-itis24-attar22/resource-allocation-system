#ifndef CAPACITYRULE_H
#define CAPACITYRULE_H

#include "../models/OneTimeRequest.h"
#include "../models/RecurringRequest.h"

class CapacityRule {
public:
    bool check(const OneTimeRequest& request) const;
    bool check(const RecurringRequest& request) const;
};

#endif