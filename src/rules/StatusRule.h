#ifndef STATUSRULE_H
#define STATUSRULE_H

#include "../models/Request.h"

class StatusRule {
public:
    bool check(const Request& request) const;
};

#endif