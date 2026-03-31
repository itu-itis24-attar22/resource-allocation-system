#ifndef STATUSRULE_H
#define STATUSRULE_H

#include "../models/OneTimeRequest.h"
#include "../models/RecurringRequest.h"

class StatusRule {
public:
    bool check(const OneTimeRequest& request) const;
    bool check(const RecurringRequest& request) const;
};

#endif