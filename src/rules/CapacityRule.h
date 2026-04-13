#ifndef CAPACITYRULE_H
#define CAPACITYRULE_H

#include "../models/Request.h"

class CapacityRule {
public:
    bool check(const Request& request) const;
};

#endif