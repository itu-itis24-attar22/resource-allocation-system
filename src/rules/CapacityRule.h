#ifndef CAPACITYRULE_H
#define CAPACITYRULE_H

#include "../models/OneTimeRequest.h"

class CapacityRule {
public:
    bool check(const OneTimeRequest& request) const;
};

#endif