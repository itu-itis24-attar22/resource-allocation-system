#ifndef STATUSRULE_H
#define STATUSRULE_H

#include "IRequestRule.h"

class StatusRule : public IRequestRule {
public:
    RuleEvaluationResult evaluate(Request& request) const override;
};

#endif