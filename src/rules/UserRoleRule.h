#ifndef USERROLERULE_H
#define USERROLERULE_H

#include "IRequestRule.h"

class UserRoleRule : public IRequestRule {
public:
    RuleEvaluationResult evaluate(Request& request) const override;
};

#endif