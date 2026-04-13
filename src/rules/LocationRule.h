#ifndef LOCATIONRULE_H
#define LOCATIONRULE_H

#include "../models/Request.h"

class LocationRule {
public:
    bool check(const Request& request) const;
};

#endif