#ifndef CAPACITYRULE_H
#define CAPACITYRULE_H

#include "IRequestRule.h"

class CapacityRule : public IRequestRule {
public:
    RuleEvaluationResult evaluate(Request& request) const override;
};

#endif