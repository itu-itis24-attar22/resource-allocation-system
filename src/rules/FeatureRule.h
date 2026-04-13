#ifndef FEATURERULE_H
#define FEATURERULE_H

#include "../models/Request.h"

class FeatureRule {
public:
    bool check(const Request& request) const;
};

#endif