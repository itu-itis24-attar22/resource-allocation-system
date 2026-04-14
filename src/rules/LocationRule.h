#ifndef LOCATIONRULE_H
#define LOCATIONRULE_H

#include "IRequestRule.h"

class LocationRule : public IRequestRule {
public:
    RuleEvaluationResult evaluate(Request& request) const override;
};

#endif