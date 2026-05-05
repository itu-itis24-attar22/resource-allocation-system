#ifndef REQUESTTYPERULE_H
#define REQUESTTYPERULE_H

#include "IRequestRule.h"

class RequestTypeRule : public IRequestRule {
public:
    RuleEvaluationResult evaluate(Request& request) const override;
};

#endif
